#include "disp.hpp"

Disp::Disp(const char *path, const char *name, mode_t type, int level)
{
    _type = type;
    _level = level;
    _child = NULL;
    _fail = 0;
    if (path != NULL && name != NULL) {
        strncpy(_name, name, MAX_FILE_NAME_LEN);
        strncpy(_path, path, MAX_FILE_NAME_LEN);
    }
}

Disp::~Disp() 
{
}


int Disp::insert(Disp *node)
{
    if (NULL == _child) {
        //first time insert
        _child = new My570List();
        if (NULL == _child) {
            cerr << "malloc failed.\n";
            return FALSE;
        }
        
        return _child->Append(node);
    } else {  //not first time, needs to strcmp to find out the right pos
        My570ListElem *item = NULL;
        Disp *dirnode = NULL;
        for (item = _child->First(); item != NULL; item = _child->Next(item))
        {
            dirnode = (Disp *)item->Obj();
            if (strcmp(node->_name, dirnode->_name) < 0) {
                return _child->InsertBefore((void *)node, item);
            }
        }

        //finish the for loop without return, means we get to the end
        //just Append
        return _child->Append(node);
    }
}

int Disp::readin()
{
    return readin(_path);
}

int Disp::readin(char *dirname)
{
    /*
     * in order to distinguish the topdir and following subdir,
     * use a static variable, tricky method.
     */
    static int open_cnt = 0;
    open_cnt++;
    DIR *dir = opendir(dirname);
    if (NULL == dir) {
        if (open_cnt == 1) {
            //open topdir failed, should exit program.
            cerr << "error when open topdir: " << dirname << endl;
            exit(-1);
        } else {
            //open subdir failed, no matter, treat it as empty, set the _fail
            _fail = 1;
            cerr << "error when open dir: " << dirname << endl;
            return FALSE;
        }
    }

    struct dirent * entry = NULL;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        struct stat fs;
        mode_t type;
        string cur_name(entry->d_name);
        string path(_path);
        string new_path = path + "/" + cur_name;
        if (0 == stat(new_path.c_str(), &fs)) {
            type = fs.st_mode;
        }
        if (S_ISDIR(type) || S_ISREG(type))
        {
            int level = _level + 1;
            Disp * node = new Disp(new_path.c_str(), entry->d_name, type, level);
            if (NULL == node) {
                cerr << "malloc failed.\n";
                return FALSE;
            }

            insert(node);
        }
    }

    //close the current dir before go into the subdir
    closedir(dir);

    //after read the current dir, we go through the dirs in it
    //so, looks like recursively.
    My570ListElem *it = NULL;
    Disp *dt = NULL;
    int res = 0;
    if (_child == NULL) {
        return TRUE;
    }
    for (it = _child->First(); it != NULL; it = _child->Next(it)) 
    {
        dt = (Disp *)it->Obj();
        if (S_ISDIR(dt->_type)) {
            res = dt->readin();
            if (res == FALSE) {
                cerr << "error readin." << dt->_path << endl;
            }
        }
    }

    return TRUE;
}

void Disp::printPre(uint64_t bits)
{
    if (_level == 0) {
        return;
    } 

    cout << " ";
    for (int i=1; i<_level; i++) {
        if (bits & 1<<i) {
            cout << "     ";
        } else {
            cout << "|    ";
        }
    }

    if (bits & 1<<_level) {
        cout << "\\-- ";
    } else {
        cout << "+-- ";
    }
    /*
    if (plast == 1) {
        //parent is the last node of its parent, then we dont output the '|' 
        cout << "     ";
    }
    if (last == 0) {
        cout << "+-- ";
    } else {
        cout << "\\-- ";
    }
    */

}


void Disp::output(uint64_t bits)
{
    printPre(bits);
    cout << _name;
    if (S_ISDIR(_type)) {
        cout << "/";
    }
    if (_fail == 1) {
        cout << "/";
    }
    cout << endl;

    if (_child == NULL) {
        return;
    }

    My570ListElem *item = NULL;
    Disp *dp = NULL;
    uint64_t subbits = bits;
    for (item = _child->First(); item != NULL; item = _child->Next(item))
    {
        dp = (Disp *)item->Obj();
        if (item == _child->Last()) {
            subbits |= 1<<dp->_level;
        }
        dp->output(subbits);
    }
}

void Disp::doRun()
{
    if(FALSE == readin()) {
        cerr << "readin failed.\n";
        return;
    } else {
        uint64_t bits = 0;
        output(bits);
    }
}
