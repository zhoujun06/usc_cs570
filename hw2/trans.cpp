#include "trans.hpp"

TransNode::TransNode(char type, time_t time, int32_t amount, char *desc)
{
    _type = type;
    _time = time;
    _amount = amount;
    _desc = desc;
}

TransRecord::TransRecord(istream &input) : _input(input) {}

TransRecord::~TransRecord()
{
    clear();
}

int TransRecord::checkType(char *line)
{
    if (line == NULL) {
        return FALSE;
    }

    if (strlen(line) > 1 || !(*line == '+' || *line == '-')) {
        return FALSE;
    }

    return TRUE;
}

/*
 * note that, because nunki is 32bit, so we could use this strtoul
 * otherwise, if on 64 bit machine, there will be a bug.
 * return value: on success, return the integer, or return FALSE(0)
 */
time_t TransRecord::checkTime(char *line)
{
    if (line == NULL) {
        return FALSE;
    }

    char *pend;
    //time_t tt;
    time_t tt;
    tt = strtol(line, &pend, 10);

    if (errno == ERANGE)
    {   
        cerr << "Error: time is out of range." << endl;
        return FALSE; 
    }
    if ((*pend) != '\0')
    {   
        cerr << "Error: time is malformed." << endl;
        return FALSE; 
    }  
    if (tt > time(NULL)) {
        cerr << "Error: time is in the future." << endl;
        return FALSE; 
    }

    return tt;
}

/*
 * on success, return the amount*100 as an integer
 * if failed, return FALSE
 */
int TransRecord::checkAmount(char *line)
{
    if (line == NULL) {
        return FALSE;
    }

    int len = strlen(line);
    if (len > 7 + 3) {
        cerr << "invalid amount len.\n";
        return FALSE;
    }

    int dot_num = 0;
    int pos = 0;
    for (int i=0; i<len; i++) {
        if (!isdigit(line[i])) {
            if (line[i] == '.') {
                dot_num++;
                pos = i;
            } else {
                cerr << "invalid amount digit.\n";
                return FALSE;
            }
        }    
    }

    if (dot_num > 1) {
        cerr << "invalid amount dot.\n";
        return FALSE;
    }
    if (len - pos != 3) {
        cerr << "invalid amount pos.\n";
        return FALSE;
    }

    //take the '.'
    line[pos] = line[pos+1];
    line[pos+1] = line[pos+2];
    line[pos+2] = '\0';

    unsigned long amount = strtoul(line, NULL, 10);
    if (amount > 1000000000)
    {
        cerr << "amount invalid.\n";
        return FALSE;
    }
    
    return amount;
}

int TransRecord::checkDesc(char *line)
{
    if (line == NULL) {
        return FALSE;
    }

    return TRUE;
}

int TransRecord::readLine(char *line, TransNode **node)
{
    int res = 0;
    char *ptk = NULL;

    char type;
    time_t time;
    int32_t amount;
    char *desc;

    
    ptk = strtok(line, "\t");
    if (FALSE == checkType(ptk)) {
        return FALSE;
    }
    type = *ptk;

    ptk = strtok(NULL, "\t");
    time = checkTime(ptk);
    if (time == FALSE) {
        return FALSE;
    }

    ptk = strtok(NULL, "\t");
    res = checkAmount(ptk);
    if (FALSE == res) {
        return FALSE;
    }
    amount = res;

    ptk = strtok(NULL, "\t");
    if (FALSE == checkDesc(ptk)) {
        return FALSE;
    }
    desc = ptk;

    ptk = strtok(NULL, "\t");
    if (NULL != ptk) {
        cerr << "malformed line, more than 3 <TAB>.\n"; 
        return FALSE;
    }

    *node = new TransNode(type, time, amount, desc);
    if (*node == NULL) {
        cerr << "new failed for line: " << line << endl;
        exit(-1);
    }

    return TRUE;
}

/*
 * sort by time when insert
 */
int TransRecord::insertNode(TransNode *node)
{
    int res = 0;

    if (_list.Empty()) {
        res = _list.Append((void *)node);
        if (res == FALSE) {
            cerr << "insertNode error. \n";
            return FALSE;
        }

        return TRUE;
    }

    My570ListElem *item = NULL;
    for (item = _list.First(); item != NULL; item = _list.Next(item))
    {
        if (node->_time == ((TransNode *)(item->Obj()))->_time)
        {
            cerr << "two transaction with same time.\n";
            return FALSE;
        }

        if (node->_time < ((TransNode *)(item->Obj()))->_time)
        {
            res = _list.InsertBefore((void *)node, item);
            if (res == FALSE) {
                cerr << "error when insert node.\n";
                return FALSE;
            }
            return TRUE;
        }
    }

    //after for without return, means get to the end of list, just apprend.
    res = _list.Append((void *)node);
    if (res == FALSE) {
        cerr << "insertNode error. \n";
        return FALSE;
    }

    return TRUE;
}

int TransRecord::readin()
{
    char line[MAX_LINE_LENGTH + 1] = {'\0'};
    int num = 0;
    int res = 0;

    /*
     * here, we first getline, then judge whether the eof is reached.
     * because the eof will work only after some op has been done on
     * the file. So, if we put the getline func inside the loop, after
     * we read the last line, the eof is not triggered, only after the
     * next read operation, the eof will work. That is, after we get the
     * last line, it still goes into the loop. So, we get one line first,
     * and put the getline at the end of the loop.
     */
    _input.getline(line, MAX_LINE_LENGTH);
    while (!_input.eof()) {
        num++;

        if (_input.fail()) {
            cerr << "error in input file, line " << num << endl;
            return FALSE;
        }

        TransNode * node = NULL;
        res = readLine(line, &node);
        if (res == FALSE) {
            cerr << "wrong format in file, line " << num << endl;
            return FALSE;
        }

        res = insertNode(node);
        if (res == FALSE) {
            cerr << "error when process input file, line " << num << endl;
            return FALSE;
        }

        _input.getline(line, MAX_LINE_LENGTH);
    }
    
    return TRUE;
}

void TransRecord::printHeader()
{
    cout << "+-----------------+--------------------------+----------------+----------------+\n";
    cout << "|       Date      | Description              |         Amount |        Balance |\n";
    cout << "+-----------------+--------------------------+----------------+----------------+\n";

}
void TransRecord::printFooter()
{
    cout << "+-----------------+--------------------------+----------------+----------------+\n";
}

void TransRecord::printDate(TransNode * node)
{
    const int LEN = 16;
    char line[LEN];
    memset(line, ' ', sizeof(line));

    char *str = ctime(&node->_time);
    strncpy(line, str, 11);
    strncpy(line+11, str+20, 4);
    *(line+15) = '\0';
    cout << "| " << line << " ";
}

void TransRecord::printDesc(TransNode * node)
{
    const int LEN = 24;
    char line[LEN + 1];
    memset(line, ' ', sizeof(line));
    int len = strlen((node->_desc).c_str());
    int size = len < LEN ? len : LEN;

    strncpy(line, (node->_desc).c_str(), size);
    line[LEN] = '\0';
    cout << "| " << line << " ";
}

void TransRecord::printAmount(TransNode * node)
{
    cout << "| ";
    printNum(node->_amount, node->_type);
    cout << " ";
}

void TransRecord::printBalance(int balance)
{
    char type = '+';
    if (balance < 0) {
        balance = -balance;
        type = '-';
    }

    cout << "| ";
    printNum(balance, type);
    cout << " |\n";
}

void TransRecord::printNum(int num, char type)
{
    const int LEN = 14;
    char line[LEN + 1];
    char single[2];

    memset(line, ' ', sizeof(line));
    line[LEN] = '\0';

    if (type == '-') {
        line[0] = '(';
        line[13] = ')';
    }

    if (num >= 1000000000)
    {
        snprintf(line+1, 12, "%s", "?,???,???.??");
    } else {
        int pos = 12;
        while (num > 0) {
            if (pos == 10) {
                line[pos--] = '.';
                continue;
            } else if (pos == 6) {
                line[pos--] = ',';
                continue;
            } else if (pos == 2) {
                line[pos--] = ',';
                continue;
            }
            
            int c = num % 10;
            snprintf(single, sizeof(single), "%d", c);
            line[pos--] = single[0];
            num /= 10;
        }
    }

    cout << line;
}

void TransRecord::printRecord()
{
    My570ListElem *item = NULL;
    TransNode *node = NULL;
    int balance = 0;
    for (item = _list.First(); item != NULL; item = _list.Next(item))
    {
        node = (TransNode *)item->Obj();
        if (node->_type == '+') {
            balance += node->_amount;
        } else if (node->_type == '-'){
            balance -= node->_amount;
        } else {
            cerr << "error transaction type\n";
            exit(-1);
        }

        printDate(node);
        printDesc(node);
        printAmount(node);
        printBalance(balance);
    }
}

void TransRecord::output()
{
    printHeader();
    printRecord();
    printFooter();
}


void TransRecord::doRun()
{
    if (FALSE == readin()) {
        return;
    }
    output();
}

void TransRecord::clear()
{}


