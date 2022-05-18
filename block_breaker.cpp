/* Code developed by Harsh Poonia */
#include <FL/Fl.H> // needed in every fltk program
#include <FL/Fl_Window.H> // needed to use the Fl_Window class
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <cmath>
#include <vector>
#include <ctime>
#include <random>
using namespace std;

namespace Velocities{
    int vx=100;
    int vy=100;
}
class Border : public Fl_Widget {
    int x, y, w, h;
    public:
        Border(int xc, int yc, int wd, int ht):Fl_Widget(0,0,1,1){
            x=xc; y=yc; w=wd; h=ht;
        }
        void draw(){
            fl_line_style(FL_SOLID, 3);
            fl_color(223);
            fl_rect(5, 5, 990, 990);
        }
};

class Rectangle : public Fl_Widget {
    protected:
        int x, y, w, h, col;
    public:
        Rectangle(int xc, int yc, int wd, int ht, int color) : Fl_Widget(xc, yc, wd, ht){
            x=xc, y=yc, w=wd, h=ht; col=color;
        }
        virtual void draw(){
            drawrectangle();
        }
        virtual void drawrectangle(){
            fl_rectf(x, y, w, h, col);
            fl_line_style(FL_SOLID, 3);
            fl_color(FL_BLACK);
            fl_rect(x, y, w, h);
        }
        int getX(){return x;}
        int getY(){return y;}
        int getWidth(){return w;}
        int getHeight(){return h;}
};

class Paddle : public Rectangle {
    
    public:
        Paddle(int xc, int yc, int wd, int ht) : Rectangle(xc-wd, yc-ht, 2*wd, 2*ht, 55){
            x=xc; y=yc; w=wd; h=ht;
        }
        
        void move(char c){
            if (c=='r'){
                x=x+25;
                if (x+w>995){x=995-w;}
            }
            else if (c=='l'){
                x=x-25;
                if (x<5){x=5;}
            }
            hide();
            resize(x-w, y-h, 2*w, 2*h);
            show();
        }
        
        virtual int handle(int e){
            if (e==FL_KEYDOWN){
                if (Fl::event_key(FL_Left)){
                    move('l');
                }
                else if (Fl::event_key(FL_Right)){
                    move('r');
                }
            }
            return 1;
        }
};

class Ball : Fl_Widget {
    double x, y, r; double vx, vy;
    public:
        Ball(int xc, int yc, int rd, int time) : Fl_Widget(xc-rd, yc-rd, 2*rd, 2*rd){
            x=xc; y=yc; r=rd; vx= Velocities::vx; vy=Velocities::vy;
        }
        void draw(){
            drawball();
        }
        void drawball(){
            fl_color(215);
            fl_pie(x-r, y-r, 2*r, 2*r, 0, 360);
        }
        void move(double dt){
            hide();
            x=x+vx*dt;
            y=y+vy*dt;
            if (y<30){
                vy=Velocities::vy;
            }
            if (x>950||x<50){
                if (x>950) vx=-Velocities::vx;
                else vx=Velocities::vx;
            }
            resize(x-r, y-r, 2*r, 2*r);
            show();
        }
        double getX(){
            return x;
        }
        double getY(){
            return y;
        }
        double getR(){
            return r;
        }
        void setVx(int x){
            vx=x;
        }
        void setVy(int y){
            vy=y;
        }
};

class Block : public Rectangle {
    
    public:
        Block(int xc, int yc, int wd, int ht, int color) : Rectangle(xc, yc, wd, ht, color){
            x=xc, y=yc, w=wd, h=ht; col=color;
        }
        
};

int collide(Block* &rect, Ball* &circ){
    //1 is top, 2 is right, 3 is down, 4 is left
    double yc=circ->getY(); double xc=circ->getX(); double rc=circ->getR();
    int x=rect->getX(); int y=rect->getY(); int w=rect->getWidth(); int h=rect->getHeight(); 
    if (yc-rc<y+h && yc-rc>y && xc<x+w && xc>x){
        return 3;
    }
    if (yc+rc<y+h && yc+rc>y && xc<x+w && xc>x){
        return 1;
    }
    if (xc-rc<x+w && xc-rc>x && yc<y+h && yc>y){
        return 2;
    }
    if (xc+rc<x+w && xc+rc>x && yc<y+h && yc>y){
        return 4;
    }
    
    return 0;
}

class Game : public Fl_Widget {
    Border* border; //
    Paddle* paddle; //
    Ball* ball; //
    vector<Block*> blocks;
    public:
        Game(Border* bdr, Paddle* p, Ball* b, vector<Block*> &vecblock) : Fl_Widget(2, 2, 1, 1){
            border = bdr;
            paddle = p;
            paddle->take_focus();
            ball = b;
            blocks=vecblock;
            
        }
        int play(){
            while (true) {
                ball->move(0.025);
                for (int i=0; i<blocks.size(); i+=1){
                    int collision=collide(blocks[i], ball);
                    if (collision==0){continue;}
                    else if (collision==1){
                        ball->setVy(-Velocities::vy);
                    }
                    else if (collision==2){
                        ball->setVx(Velocities::vx);
                    }
                    else if (collision==3){
                        ball->setVy(Velocities::vy);
                    }
                    else if (collision==4){
                        ball->setVx(-Velocities::vx);
                    }
                    delete blocks[i];
                    blocks.erase(blocks.begin()+i);
                }
                if (ball->getY()+ball->getR()>875){
                    if ((ball->getX()<paddle->getX()+paddle->getWidth()) && (ball->getX()>paddle->getX()-paddle->getWidth())){
                        ball->setVy(-Velocities::vy);
                    }
                    else break;
                }
                if (blocks.size()==0){
                    break;
                }
                
                Fl::wait(0.01);
                
            }
            if (blocks.size()==0){
                return 1;
            }
            while (ball->getY() <1000){
                ball->move(0.05);
                Fl::wait(0.02);
            }
            
            return 0;
        }
        
        void draw(){}
};

class Text : public Fl_Widget {
    const char* text; int x, y, w, h;
    public:
        Text(const char* txt, int xc, int yc, int wd, int ht):Fl_Widget(xc, yc, wd, ht){
            text=txt; x=xc; y=yc; w=wd; h=ht; 
        }
        void draw(){
            drawtext();
        }
        void drawtext(){
            fl_rectf(x, y, w, h, 134);
            fl_rect(x, y, w, h, 32);
            fl_font(FL_COURIER, 56);
            fl_draw(text, x, y, w, h, FL_ALIGN_CENTER);
        }
        
};

int main(){
    srand(0);
    Fl_Window w(1000, 1000, "Block Breaker");
    w.color(176);
    Border border(5, 5, 990, 990);
    Paddle paddle(400, 875, 200, 50);
    Ball ball(100 + 10*(rand()%80),500, 25, 0.01);
    vector<Block*> blocks;
    for (int i=0; i<30; i+=1){
        switch (i/6)
        {
        case 0:
            blocks.push_back(new Block(140+120*(i%6), 200+50*(i/6), 120, 50, 175-(i%6)));
            break;
            
        case 1:
            blocks.push_back(new Block(140+120*(i%6), 200+50*(i/6), 120, 50, 98+(i%6)));
            break;
        case 2:
            blocks.push_back(new Block(140+120*(i%6), 200+50*(i/6), 120, 50, 223));
            break;
        case 3:
            blocks.push_back(new Block(140+120*(i%6), 200+50*(i/6), 120, 50, 249+(i%6)));
            break;
        case 4:
            blocks.push_back(new Block(140+120*(i%6), 200+50*(i/6), 120, 50, 134-(i%6)));
            break;
        
        }
    }

    Game game(&border, &paddle, &ball, blocks); //make the game
    
    w.show();

    Fl_Window loss(1000, 1000, "Game Over"); //window displayed when game lost
    loss.color(129);
    Text t("GAME OVER", 250, 400, 500, 200);
    loss.add(&t);

    Fl_Window win(1000, 1000, "Game Won"); //window displayed whn game won
    win.color(63);
    Text t2("CONGRATULATIONS", 200, 400, 600, 200);
    win.add(&t2);
    
    int res=game.play();
    if (res==0){
        w.hide();
        loss.show();
    }
    else if (res==1){
        w.hide();
        win.show();
    }

    Fl::run();
}
