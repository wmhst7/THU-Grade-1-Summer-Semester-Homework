#include "kernel.h"

Cube::Cube(){
    kind = null;
    black = true;
    moved = false;
}

Kernel::Kernel(QObject *parent) : QObject(parent)
{
    black = false;
    NowPoint = QPoint(0, 0);
    ToKind = queen;
    status = 4;
    Cubes[1][1].kind = rook;
    Cubes[2][1].kind = knight;
    Cubes[3][1].kind = bishop;
    Cubes[4][1].kind = queen;
    Cubes[5][1].kind = king;
    Cubes[6][1].kind = bishop;
    Cubes[7][1].kind = knight;
    Cubes[8][1].kind = rook;
    Cubes[1][8].kind = rook;
    Cubes[2][8].kind = knight;
    Cubes[3][8].kind = bishop;
    Cubes[4][8].kind = queen;
    Cubes[5][8].kind = king;
    Cubes[6][8].kind = bishop;
    Cubes[7][8].kind = knight;
    Cubes[8][8].kind = rook;
    for(int i = 1;i<9;i++)
        Cubes[i][2].kind = pawn;
    for(int i = 1;i<9;i++){
        Cubes[i][7].kind = pawn;
        Cubes[i][7].black = false;
        Cubes[i][8].black = false;
    }
}

void Kernel::Initial(){
    black = false;
    NowPoint = QPoint(0, 0);
    ToKind = queen;
    status = 4;
    Cubes[1][1].kind = rook;
    Cubes[2][1].kind = knight;
    Cubes[3][1].kind = bishop;
    Cubes[4][1].kind = queen;
    Cubes[5][1].kind = king;
    Cubes[6][1].kind = bishop;
    Cubes[7][1].kind = knight;
    Cubes[8][1].kind = rook;
    Cubes[1][8].kind = rook;
    Cubes[2][8].kind = knight;
    Cubes[3][8].kind = bishop;
    Cubes[4][8].kind = queen;
    Cubes[5][8].kind = king;
    Cubes[6][8].kind = bishop;
    Cubes[7][8].kind = knight;
    Cubes[8][8].kind = rook;
    for(int i = 1;i<9;i++)
        Cubes[i][2].kind = pawn;
    for(int i = 1;i<9;i++){
        Cubes[i][7].kind = pawn;
        Cubes[i][7].black = false;
        Cubes[i][8].black = false;
    }
}

void Kernel::UpdateAblePoints(){
    AblePoints.clear();
    if(NowPoint == QPoint(0, 0)) return;
    Cube & c = Cubes[NowPoint.x()][NowPoint.y()];
    int x = NowPoint.x();
    int y = NowPoint.y();
    if(c.kind == null) return;
    if(c.kind == king){
        if(Cubes[x+1][y-1].kind == null && In(x+1, y-1))
            AblePoints<<QPoint(x+1, y-1);
        if(Cubes[x+1][y].kind == null && In(x+1, y))
            AblePoints<<QPoint(x+1, y);
        if(Cubes[x+1][y+1].kind == null && In(x+1, y+1))
            AblePoints<<QPoint(x+1, y+1);

        if(Cubes[x][y-1].kind == null && In(x, y-1))
            AblePoints<<QPoint(x, y-1);
        if(Cubes[x][y].kind == null && In(x, y))
            AblePoints<<QPoint(x, y);
        if(Cubes[x][y+1].kind == null && In(x, y+1))
            AblePoints<<QPoint(x, y+1);

        if(Cubes[x-1][y-1].kind == null && In(x-1, y-1))
            AblePoints<<QPoint(x-1, y-1);
        if(Cubes[x-1][y].kind == null && In(x-1, y))
            AblePoints<<QPoint(x-1, y);
        if(Cubes[x-1][y+1].kind == null && In(x-1, y+1))
            AblePoints<<QPoint(x-1, y+1);
    }else if(c.kind == rook){
        for(int i = 1;i<9;i++){
            if(Cubes[i+x][y].kind == null && In(i+x, y))
                AblePoints<<QPoint(i+x, y);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y].kind == null && In(x-i, y))
                AblePoints<<QPoint(x-i, y);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x][y+i].kind == null && In(x, y+i))
                AblePoints<<QPoint(x, y+i);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x][y-i].kind == null && In(x, y-i))
                AblePoints<<QPoint(x, y-i);
            else
                break;
        }
    }else if(c.kind == bishop){
        for(int i = 1;i<9;i++){
            if(Cubes[i+x][y+i].kind == null && In(x+i, y+i))
                AblePoints<<QPoint(x+i, y+i);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y+i].kind == null && In(x-i, y+i))
                AblePoints<<QPoint(x-i, y+i);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[i+x][y-i].kind == null && In(x+i, y-i))
                AblePoints<<QPoint(x+i, y-i);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y-i].kind == null && In(x-i, y-i))
                AblePoints<<QPoint(x-i, y-i);
            else
                break;
        }
    }else if(c.kind == queen){
        for(int i = 1;i<9;i++){
            if(Cubes[i+x][y+i].kind == null && In(x+i, y+i))
                AblePoints<<QPoint(x+i, y+i);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y+i].kind == null && In(x-i, y+i))
                AblePoints<<QPoint(x-i, y+i);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[i+x][y-i].kind == null && In(x+i, y-i))
                AblePoints<<QPoint(x+i, y-i);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y-i].kind == null && In(x-i, y-i))
                AblePoints<<QPoint(x-i, y-i);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[i+x][y].kind == null && In(i+x, y))
                AblePoints<<QPoint(i+x, y);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y].kind == null && In(x-i, y))
                AblePoints<<QPoint(x-i, y);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x][y+i].kind == null && In(x, y+i))
                AblePoints<<QPoint(x, y+i);
            else
                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x][y-i].kind == null && In(x, y-i))
                AblePoints<<QPoint(x, y-i);
            else
                break;
        }
    }else if(c.kind == knight){
        for(int i = 1;i<9;i++){
            for(int j = 1;j<9;j++){
                if((qAbs(i - x) == 1 && qAbs(j - y) == 2) ||(qAbs(i - x) == 2 && qAbs(j - y) == 1)){
                    if(Cubes[i][j].kind == null && In(x, i))
                        AblePoints<<QPoint(i,j);
                }
            }
        }
    }else if(c.kind == pawn){
        if(c.black){
            if(Cubes[x][y+1].kind == null && In(x, y+1)){
                AblePoints<<QPoint(x,y+1);
                if(!c.moved && Cubes[x][y+2].kind == null && In(x, y+2))
                    AblePoints<<QPoint(x,y+2);
            }
        }else{
            if(Cubes[x][y-1].kind == null && In(x, y-1)){
                AblePoints<<QPoint(x,y-1);
                if(!c.moved && Cubes[x][y-2].kind == null && In(x, y-2))
                    AblePoints<<QPoint(x,y-2);
            }
        }
    }



    MovePoints.clear();
    if(c.kind == king){
        if(Cubes[x+1][y-1].kind != null && In(x+1, y-1))
            MovePoints<<QPoint(x+1, y-1);
        if(Cubes[x+1][y].kind != null && In(x+1, y))
            MovePoints<<QPoint(x+1, y);
        if(Cubes[x+1][y+1].kind != null && In(x+1, y+1))
            MovePoints<<QPoint(x+1, y+1);

        if(Cubes[x][y-1].kind != null && In(x, y-1))
            MovePoints<<QPoint(x, y-1);
        if(Cubes[x][y].kind != null && In(x, y))
            MovePoints<<QPoint(x, y);
        if(Cubes[x][y+1].kind != null && In(x, y+1))
            MovePoints<<QPoint(x, y+1);

        if(Cubes[x-1][y-1].kind != null && In(x-1, y-1))
            MovePoints<<QPoint(x-1, y-1);
        if(Cubes[x-1][y].kind != null && In(x-1, y))
            MovePoints<<QPoint(x-1, y);
        if(Cubes[x-1][y+1].kind != null && In(x-1, y+1))
            MovePoints<<QPoint(x-1, y+1);
    }else if(c.kind == rook){
        for(int i = 1;i<9;i++){
            if(Cubes[x+i][y].kind != null && In(x+i, y)){
                MovePoints<<QPoint(x+i, y);
                break;
            }

        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y].kind != null && In(x-i, y))
                MovePoints<<QPoint(x-i, y);

                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x][y+i].kind != null && In(x, y+i))
                MovePoints<<QPoint(x, y+i);

                break;
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x][y-i].kind != null && In(x, y-i))
                MovePoints<<QPoint(x, y-i);

                break;
        }
    }else if(c.kind == bishop){
        for(int i = 1;i<9;i++){
            if(Cubes[i+x][y+i].kind != null && In(x+i, y+i)){
                MovePoints<<QPoint(x+i, y+i);
                break;
            }

        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y+i].kind != null && In(x-i, y+i)){
                MovePoints<<QPoint(x-i, y+i);

                break;}
        }
        for(int i = 1;i<9;i++){
            if(Cubes[i+x][y-i].kind != null && In(x+i, y-i)){
                MovePoints<<QPoint(x+i, y-i);

                break;}
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y-i].kind != null && In(x-i, y-i)){
                MovePoints<<QPoint(x-i, y-i);

                break;}
        }
    }else if(c.kind == queen){
        for(int i = 1;i<9;i++){
            if(Cubes[i+x][y+i].kind != null && In(x+i, y+i)){
                MovePoints<<QPoint(x+i, y+i);

                break;}
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y+i].kind != null && In(x-i, y+i)){
                MovePoints<<QPoint(x-i, y+i);

                break;}
        }
        for(int i = 1;i<9;i++){
            if(Cubes[i+x][y-i].kind != null && In(x+i, y-i)){
                MovePoints<<QPoint(x+i, y-i);

                break;}
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y-i].kind != null && In(x-i, y-i)){
                MovePoints<<QPoint(x-i, y-i);

                break;}
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x+i][y].kind != null && In(x+i, y)){
                MovePoints<<QPoint(x+i, y);

                break;}
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x-i][y].kind != null && In(x-i, y)){
                MovePoints<<QPoint(x-i, y);

                break;}
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x][y+i].kind != null && In(x, y+i)){
                MovePoints<<QPoint(x, y+i);

                break;}
        }
        for(int i = 1;i<9;i++){
            if(Cubes[x][y-i].kind != null && In(x, y-i)){
                MovePoints<<QPoint(x, y-i);

                break;}
        }
    }else if(c.kind == knight){
        for(int i = 1;i<9;i++){
            for(int j = 1;j<9;j++){
                if((qAbs(i - x) == 1 && qAbs(j - y) == 2) ||(qAbs(i - x) == 2 && qAbs(j - y) == 1)){
                    if(Cubes[i][j].kind != null && In(x, i))
                        MovePoints<<QPoint(i,j);
                }
            }
        }
    }else if(c.kind == pawn){
        if(c.black){
            if(Cubes[x+1][y+1].kind != null && In(x+1, y+1)){
                MovePoints<<QPoint(x+1,y+1);
            }
            if(Cubes[x-1][y+1].kind != null && In(x-1, y+1)){
                MovePoints<<QPoint(x-1,y+1);
            }
        }else{
            if(Cubes[x-1][y-1].kind != null && In(x-1, y-1)){
                MovePoints<<QPoint(x-1,y-1);
            }
            if(Cubes[x+1][y-1].kind != null && In(x+1, y-1)){
                MovePoints<<QPoint(x+1,y-1);
            }
        }
    }
}

bool Kernel::Move(QPoint p1, QPoint p2){
    int x = p1.x(); int y = p1.y();
    int xx = p2.x(); int yy = p2.y();
    if(!In(x, y) || !In(xx, yy)) return false;
    if(GetCube(p1).black != black) return false;
    if(GetCube(p1).kind == null) return false;
    for(auto it : AblePoints){
        if(p2 == it){
            GetCube(p2) = GetCube(p1);
            GetCube(p1).Init();
            black == true ? black = false : black = true;
            return true;
        }
    }
    for(auto it : MovePoints){
        if(p2 == it){
            if((GetCube(p1).black && !GetCube(p2).black)
                    || (!GetCube(p1).black && GetCube(p2).black)){
                GetCube(p2) = GetCube(p1);
                GetCube(p1).Init();
                black == true ? black = false : black = true;
                return true;
            }
        }
    }
    return false;
}

Cube& Kernel::GetCube(QPoint p){
    if(In(p.x(), p.y())) return Cubes[p.x()][p.y()];
}

Cube& Cube::operator=(const Cube& c){
    this->kind = c.kind;
    this->black = c.black;
    this->moved = true;
    return *this;
}

void Cube::Init(){
    kind = null;
    black = true;
    moved = false;
}









