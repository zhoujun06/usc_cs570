#ifndef _TRANS_HPP_
#define _TRANS_HPP_

#include "hw2_include.h"

using namespace std;

class TransRecord;

class TransNode
{
    public:
        TransNode(char type, time_t time, int32_t amount, char *desc);
        ~TransNode();

    private:
        friend class TransRecord;

        char _type;
        time_t _time;
        int32_t _amount;
        string _desc;
};

const int MAX_LINE_LENGTH = 1024;
class TransRecord
{
    public:
        TransRecord(istream &input);
        ~TransRecord();

        void doRun();

    private:
        int readin();
        void output();
        void clear();

        int checkType(char *line);
        time_t checkTime(char *line);
        int checkAmount(char *line);
        int checkDesc(char *line);
        int readLine(char *line, TransNode **node);
        int insertNode(TransNode *node);
        void printHeader();
        void printFooter();
        void printDate(TransNode * node);
        void printDesc(TransNode * node);
        void printAmount(TransNode * node);
        void printBalance(int balance);
        void printNum(int num, char type);
        void printRecord();

    private:
        My570List _list;
        istream &_input;
};
#endif
