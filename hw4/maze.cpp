#include "maze.hpp"

extern bool g_info;

Maze::~Maze()
{
    delete [] _data[0];
    delete [] _data;
}

Maze::Maze(FILE * fp, int height, int width, 
            int top, int bottom,
            int left, int right)
{
    _fp = fp;
    _height = height;
    _width = width;
    _top = top;
    _bottom = bottom;
    _left = left;
    _right = right;
}

void Maze::initMaze()
{
    initData();
    initGate();
    initWallList();
    initRoom();
}

void Maze::initData()
{
    int row = getRowNum();
    int col = getColNum();

    _data = new char *[row];
    _data[0] = new char[row*col];
    for(int i=1; i<row; i++)
    {
        _data[i] = _data[i-1]+col;
    }

    memset(_data[0], ' ', row*col);

    for(int i=0; i<row; i=i+2) {
        for (int j=0; j<col; j=j+2) {
            _data[i][j] = '+';
        }
    }

    for(int i=0; i<row; i=i+2) {
        for (int j=1; j<col; j=j+2) {
            _data[i][j] = '-';
        }
    }

    for(int i=1; i<row; i=i+2) {
        for (int j=0; j<col; j=j+2) {
            _data[i][j] = '|';
        }
    }
}

void Maze::initGate()
{
    if (_top == 0 && _bottom == 0 && _left == 0 && _right == 0) {
        _data[0][1] = ' ';
        _data[2*_height][2*_width-1] = ' ';
    } else {
        if (_top != 0) {
            _data[0][2*_top-1] = ' ';
        }
        if (_bottom != 0) {
            _data[2*_height][2*_bottom-1] = ' ';
        }
        if (_left != 0) {
            _data[2*_left-1][0] = ' ';
        }
        if (_right != 0) {
            _data[2*_right-1][2*_width] = ' ';
        }
    }
}

void Maze::initWallList()
{
    int num = _height * (_width -1) + _width * (_height - 1);
    for (int i=0; i< num; i++) {
        Wall *node = new Wall(i);
        _list.Append((void *)node);
    }
}

void Maze::initRoom()
{
    int room_num = _height * _width;

    for (int i=0; i<room_num; i++) {
        _room.push_back(Room(i));
    }
}

int Maze::getCoFromWallNum(int num, int&row, int &col)
{
    int p = num / (2*_width -1);
    int r = num % (2*_width -1);
    
    if (r == 0) {
        row = 2*p + 1;
        col = 2;
    } else {
        if (r < _width -1 ) {
            row = 2*p + 1;
            col = 2*(r+1);
        } else {
            row = 2*p + 2;
            col = 2*(r-(_width-1))+1;
        }
    }

    return 0;
}

bool Maze::getWallNum(uint32_t &num)
{
    unsigned char bin;
    size_t sz = 0;
    for (size_t i=0; i<4; i++) {
        sz = fread(&bin, 1, 1, _fp);
        if (sz != 1) {
            return false;
        }
        num = num << 8;
        num |= bin;
       // printf("%X ", bin);
    }

    return true;
}

int Maze::removeWall()
{
    /*  
    fseek(_fp, 0, SEEK_END);
    long size = ftell(_fp);
    rewind(_fp);
    if (size == -1) {
        cerr << "please input bin data for wall\n";
        exit(1);
    }

    unsigned char *buf = (unsigned char *)malloc(sizeof(unsigned char)*size);
    if (buf == NULL) {
        cerr << "malloc failed for data, size " << size << endl;
        exit(1);
    }
    long res;
    res = fread(buf, 1, size, _fp);
    if (res != size) {
        cerr << "read data failed.\n";
        exit(1);
    }
    */

    //int i = 0;
    uint32_t num;
    int removed_num = 0;
/*    while (i < size) {
        num = buf[i];

        for (int j=1; j<4; j++) {
            num = num << 8;
            num |= buf[i+j];
        }
        i += 4;
*/
    while (!feof(_fp)) {
        if (false == getWallNum(num)) {
            break;
        }

        My570ListElem *item = _list.First();

        item = _list.First();
        num = num % _list.Length();
        for (uint32_t i=0; i<num; i++) {
            item = _list.Next(item);
        }
        int wallNum = ((Wall *)(item->Obj()))->_num;

        int row = 0;
        int col = 0;

        getCoFromWallNum(wallNum, row, col);

        int room1 = 0;
        int room2 = 0;
        getAdjRoom(row, col, room1, room2);

        if (false == isConnected(room1, room2)) {
            _data[row][col] = ' ';
            if (g_info) {
                cout << "Wall " << wallNum << " removed." << endl;
            }

            if (++removed_num >= _height*_width - 1) {
                break;
            }
        }
        _list.Unlink(item);

    }

    if (removed_num < _height*_width - 1) {
        cerr << "not enough wall removed, bad input file\n";
        exit(1);
    }

    return 0;
}

int Maze::getAdjRoom(int row, int col, int &room1, int &room2)
{
    if (row % 2 == 1) {
        //odd row,room on left and right
        //room start from 0, not 1
        room1 = (row/2 * _width - 1) + (col/2);
        room2 = room1 + 1;
    } else {
        //even row, room on top and bottom
        room1 = ((row/2-1) * _width -1) + (col+1)/2;
        room2 = room1 + _width;
    }

    return 0;
}

bool Maze::isConnected(int room1, int room2)
{
    Room *r1 = &(_room[room1]);
    Room *r2 = &(_room[room2]);

    int level1 = 0;
    int level2 = 0;
    Room *pr1 = NULL;
    Room *pr2 = NULL;

    r1->getRoot(pr1, level1);
    r2->getRoot(pr2, level2);

    if (pr1 == pr2) {
        return true;
    } else {
        if (level1 < level2) {
            pr1->_parent = pr2; 
        } else {
            pr2->_parent = pr1; 
        }
        return false;
    }
}

int Room::getRoot(Room * &p, int &level)
{
    level = 1;
    p = _parent;
    if (p == NULL) {
        p = this;
        return 0;
    }

    while (p->_parent != NULL) {
        level++;
        p = p->_parent;
    }

    return 0;
}

void Maze::printMaze()
{
    int row = getRowNum();
    int col = getColNum();

    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            cout << _data[i][j];
        }
        cout << endl;
    }
}

void Maze::build()
{
    initMaze();
    removeWall();
    printMaze();
}

