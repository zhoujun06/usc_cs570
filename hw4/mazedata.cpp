#include "mazedata.hpp"

int State::getPathFromRoot(std::vector<State *> &path)
{
    std::stack<State *> tmp;
    State *p = this;
    while (p != NULL) {
        tmp.push(p);
        p = p->_parent;
    }

    while (!tmp.empty()) {
        p = tmp.top();
        path.push_back(p);
        tmp.pop();
    }

    return 0;
}

MazeData::MazeData(istream & input) : _input(input) {}
MazeData::~MazeData()
{
}

int MazeData::getIndex(int row, int col)
{
    return row*_width + col;
}

char *MazeData::trim(char *str)
{
    if (str == NULL) {
        return NULL;
    }

    char *head = str;
    char *end = str + strlen(str)-1;
    while (head != NULL && 
        (*head == ' ' || *head == '\t' || *head == '\r'))
    {
        head++;
    }

    if (head == NULL) 
    {
        return NULL;
    }

    while (*end == ' ' || *end == '\t' || *end == '\r')
    {
        end--;
    }

    *(end+1) = '\0';

    return head;
}

void MazeData::init()
{
    int row = 0;
    int col = 0;
    int len = 0;
    char line[1024]={'\0'};

    _input.getline(line, sizeof(line));
    col = strlen(line);
    if (col % 2 == 0) {
        cerr << "line should be odd length.\n";
        exit(1);
    }

    while (!_input.eof()) {
        len = strlen(line);
        if (len != col) {
            cerr << "lines should be of the same length.\n";
            exit(1);
        }

        char *item = new char[len+1];
        strncpy(item, line, len);

        _data.push_back(item);
        _input.getline(line, sizeof(line));
        row++;
    }

    if (row % 2 == 0) {
        cerr << "should be odd numbers of lines.\n";
        exit(1);
    }

    initParam();
    checkInvalidChar();
    changeToGraph();
}

void MazeData::checkInvalidChar()
{
    //check room
    for (int i=1; i<=2*_height; i+=2) {
        for (int j=1; j<=2*_width; j+=2) {
            if (_data[i][j] != ' ') {
                cerr << "Invalid charecter in room.\n";
                cerr << "position: row:" << i << " col:" << j << endl;
                exit(1);
            }
        }
    } 
    //check conor
    for (int i=0; i<=2*_height; i+=2) {
        for (int j=0; j<=2*_width; j+=2) {
            if (_data[i][j] != '+') {
                cerr << "Invalid charecter in cornor.\n";
                cerr << "position: row:" << i << " col:" << j << endl;
                exit(1);
            }
        }
    } 
    //check horizontal edge
    for (int i=0; i<=2*_height; i+=2) {
        for (int j=1; j<=2*_width; j+=2) {
            if (!(_data[i][j] == '-' || _data[i][j] == ' ' ||
                 (_data[i][j] >= '0' && _data[i][j] <= '9' ))) {
                cerr << "Invalid charecter in horizontal edge.\n";
                cerr << "position: row:" << i << " col:" << j << endl;
                exit(1);
            }
        }
    } 

    //check vertical edge
    for (int i=1; i<=2*_height; i+=2) {
        for (int j=0; j<=2*_width; j+=2) {
            if (_data[i][j] != '|' && _data[i][j] != ' ' &&
                !(_data[i][j] >= '0' && _data[i][j] <= '9' )) {
                cerr << "Invalid charecter in vertical edge.\n";
                cerr << "position: row:" << i << " col:" << j << endl;
                exit(1);
            }
        }
    } 

}

/*
 */
void MazeData::initParam()
{
    int height = _data.size();
    int width = strlen(_data[0]);

    _height = height / 2;
    _width = width / 2;

    /*
     * check type
     */
    _type = MAZE_TYPE_1;
    for (int i=1; i<height; i+=2 ) {
        for (int j=2; j<width; j+=2) {
            if (_data[i][j] >= '0' && _data[i][j] <= '9') {
                _type = MAZE_TYPE_2;
                break;
            }
        }
        break;
    }

    int num = 0;
    int room_num = 0;

    for (int i=1; i<width; i+=2) {
        if (_data[0][i] == ' ') {
            room_num = i/2;
            _entry[num++] = room_num;
            if (num > 2) {
                cerr << "too many opennings, should be 2\n";
                exit(1);
            }
        }

        if (_data[height-1][i] == ' ') {
            room_num = _width * (_height-1) + i/2;
            _entry[num++] = room_num;
            if (num > 2) {
                cerr << "too many opennings, should be 2\n";
                exit(1);
            }
        }
    }

    for (int i=1; i<height; i+=2) {
        if (_data[i][0] == ' ') {
            room_num = _width * (i/2);
            _entry[num++] = room_num; 
            if (num > 2) {
                cerr << "too many opennings, should be 2\n";
                exit(1);
            }
        }

        if (_data[i][width-1] == ' ') {
            room_num = (_width * (i+1)/2-1);
            _entry[num++] = room_num;
            if (num > 2) {
                cerr << "too many opennings, should be 2\n";
                exit(1);
            }
        }
    }

    if (_entry[0] > _entry[1]) {
        int tmp = _entry[0];
        _entry[0] = _entry[1];
        _entry[1] = tmp;
    }
}

int MazeData::checkNb(int i, int j, int ni, int nj)
{
    int edge_row = (i + ni)/2;
    int edge_col = (j + nj)/2;
    int cost = 0;

    if (_data[edge_row][edge_col] == '-' || _data[edge_row][edge_col] == '|') {
        //no way
    } else {
        if (_type == MAZE_TYPE_1) {
            cost = 1;
        } else {
            cost = _data[edge_row][edge_col] - 48;
        }

        Edge * edge = new Edge(cost);

        int room_num = getIndex(i/2, j/2);
        int nb_num = getIndex(ni/2, nj/2);

        RoomDNb nb;
        nb._room = &(_room[nb_num]);
        nb._edge = edge;

        _room[room_num]._nbs.push_back(nb);
    }

    return 0;
}
/*
 * change to graph from raw data
 */
void MazeData::changeToGraph()
{
    /*
     * generate rooms
     */
    for(int i=0; i<_height; i++) {
        for(int j=0; j<_width; j++) {
            int num = i * _width + j;
            _room.push_back(RoomD(i, j, num));
        }
    }

    /*
     * generate room neighbors
     */

    int height = _data.size();
    int width = strlen(_data[0]);

    for(int i=1; i<height; i+=2) {
        for (int j=1; j<width; j+=2) {
            //i j is the actual index of the room in _data
            //check top neighbor
            if (i-2 > 0) {
                checkNb(i, j, i-2, j);
            }
            if (i+2 < height) {
                checkNb(i, j, i+2, j);
            }
            if (j-2 > 0) {
                checkNb(i, j, i, j-2);
            }
            if (j+2 < width) {
                checkNb(i, j, i, j+2);
            }
        } //end for (j)
    } // end for (i);
}

void MazeData::printData()
{
    for(int i=0; i<2*_height+1; i++) {
        for(int j=0; j<2*_width+1; j++) {
            cout << _data[i][j];
        }
        cout << endl;
    }
}

void MazeData::printGraph()
{
    cout << "Height: " << _height;
    cout << " Width: " << _width;
    cout << " Start: " << _entry[0];
    cout << " End: " << _entry[1] << endl;

    for(size_t i=0; i<_room.size(); i++) {
        RoomD room = _room[i];

        cout << "Room: " << room._num;
        cout << " row:" << room._row;
        cout << " col:" << room._col;
        cout << " nbs:";
        for (size_t j=0; j<room._nbs.size(); j++) {
            RoomDNb nb = room._nbs[j];
            cout << " n:" << nb._room->_num;
            cout << " r:" << nb._room->_row;
            cout << " c:" << nb._room->_col;
            cout << " edge:" << nb._edge->_cost;
        }
        cout << endl;
    }

}

void MazeData::fillPath(std::vector<State *> &path)
{
    /*
     * first to delete the digits
     */
    for (int i=1; i<2*_height; i+=2) {
        for (int j=2; j<2*_width; j+=2) {
            if (_data[i][j] != '|') {
                _data[i][j] = ' ';
            }
        }
    }
    for (int i=2; i<2*_height; i+=2) {
        for (int j=1; j<2*_width; j+=2) {
            if (_data[i][j] != '-') {
                _data[i][j] = ' ';
            }
        }
    }

    /* 
     * add the entry and end
     */
    for (int i=0; i<2; i++) {
        int s_num = _entry[i];
        int s_row = _room[s_num]._row;
        int s_col = _room[s_num]._col;

        if (s_row == 0 && _data[0][2*s_col+1] == ' ') {
           _data[0][2*s_col+1] = '@'; 
        } 
        if (s_row == _height-1 && _data[2*_height][2*s_col+1] == ' ') {
            _data[2*_height][2*s_col+1] = '@';
        }
        if (s_col == 0 && _data[2*s_row+1][0] == ' ') {
            _data[2*s_row+1][0] = '@';
        }
        if (s_col == _width-1 && _data[2*s_row+1][2*_width] == ' ') {
            _data[2*s_row+1][2*_width] = '@';
        }
    }

    /* 
     * fill path
     */
    for (size_t i=0; i<path.size(); i++) {
        int row = path[i]->_room->_row;
        int col = path[i]->_room->_col;
        _data[2*row+1][2*col+1] = '@';

        /*
         * check the edge between this node and the next one
         */
        if (i+1 < path.size()) {
            int nrow = path[i+1]->_room->_row;
            int ncol = path[i+1]->_room->_col;
            if (nrow == row+1) {
                _data[2*row+2][2*col+1] = '@';
            } else if (nrow == row-1) {
                _data[2*row][2*col+1] = '@';
            } else if (ncol == col+1) {
                _data[2*row+1][2*col+2] = '@';
            } else if (ncol == col-1) {
                _data[2*row+1][2*col] = '@';
            }
        }
    }
}

