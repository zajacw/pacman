#include <allegro.h>
#include <math.h>
#include <sstream>

////////////////////
//Global variables
BITMAP* point;
BITMAP* buffer;
BITMAP* man;
BITMAP* monster;
SAMPLE* beep;
int map[24][32] = {{1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
                   {1,1,1,1,1,1,1,1,1,1,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3},
                   {3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,1,1,1,1,1,1,1,1,3},
                   {3,1,1,1,1,1,1,1,1,3,3,3,3,3,3,1,1,1,1,1,1,1,3,3,3,1,1,3,3,1,1,3},
                   {3,1,1,3,1,1,1,1,1,1,3,1,1,1,3,3,3,3,3,3,1,1,1,3,1,1,1,3,1,1,1,3},
                   {3,1,1,3,1,1,1,1,1,1,3,3,1,1,1,1,1,1,1,3,1,1,1,3,3,3,3,3,1,1,1,3},
                   {3,1,1,3,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,1,1,1,1,3},
                   {3,1,1,3,3,3,3,3,3,1,1,3,1,1,1,3,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,3},
                   {3,1,1,3,1,1,1,1,1,1,1,3,1,1,1,3,3,3,3,3,1,1,1,1,1,3,1,1,1,1,1,3},
                   {3,1,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,3,1,1,1,1,1,3},
                   {3,1,1,3,3,3,3,3,1,1,1,3,1,1,1,1,1,1,1,3,3,3,3,3,3,3,1,1,1,1,1,3},
                   {3,1,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,3,1,1,1,1,3,1,1,1,1,1,3},
                   {3,1,1,3,3,3,3,3,3,3,1,3,3,3,3,3,3,1,1,1,3,1,1,1,1,3,1,1,1,1,1,3},
                   {3,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,3,1,1,1,3,1,3,1,1,3,1,1,1,1,1,3},
                   {3,3,3,3,3,3,3,1,1,3,1,1,1,1,1,1,3,1,1,1,3,3,3,1,1,3,3,3,1,1,1,3},
                   {3,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,3,1,1,1,3},
                   {3,1,3,1,3,1,1,1,1,3,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,3,3,3,1,1,1,3},
                   {3,1,3,1,3,1,1,1,1,3,1,1,1,1,3,1,3,3,3,3,3,3,1,1,3,1,1,1,1,1,1,3},
                   {3,1,3,1,3,1,1,1,1,3,1,1,1,1,3,1,3,1,1,1,1,3,1,1,3,1,1,1,1,1,1,3},
                   {3,1,1,1,3,1,1,1,1,3,1,1,1,1,3,1,3,1,1,1,1,3,1,1,3,3,3,3,3,3,3,3},
                   {3,1,1,1,3,3,3,1,1,3,3,3,3,3,3,1,1,3,3,3,1,1,1,1,1,1,1,1,1,1,1,3},
                   {3,1,1,1,1,1,3,1,1,1,1,1,1,1,3,1,1,1,1,3,3,1,1,1,1,1,1,1,1,1,1,3},
                   {3,1,1,1,1,1,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3},
                   {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3}};

//////////////////
//Functions initializations
int getWhole(float a) {return (int)trunc(a);}

//////////////////
//Classes
class enemy
{
      private:
          int x,y, pos;
      public:
          enemy() {pos=rand()%4+1;}
          void set( int a, int b){x=a; y=b;}
          int get_x() {return x;}
          int get_y() {return y;}
          void enMove();
};

class pacman
{
      private:
          int x,y;
      public:
          int time, pos, points;
          bool dead;
          int get_x() {return x;}
          int get_y() {return y;}
          pacman()
          { 
               dead=false;
               points=0;
               x=1;
               y=1;
          }
          bool compareCoords(enemy monster);
          void move(enemy mon[]);
          void setImage();
}player;

///////////////////////
//Methods
void enemy::enMove()
{
     int mx=getWhole(x/20);
     int my=getWhole(y/20);
     switch(pos)
     {
         case 1: //down
         {
              if (y<479)
              {
                 if(x==mx*20) //the case of one tile
                    {
                      if(map[my+1][mx]!=1&&map[my+1][mx]!=0)
                      {
                          if(rand()%2==0) pos=2;
                          else pos=4;
                      }
                    }
                 else if ((map[my+1][mx]!=1&&map[my+1][mx]!=0)&&(map[my+1][mx+1]!=1&&map[my+1][mx+1]!=0))
                 {
                          if(rand()%2==0) pos=2;
                          else pos=4;
                 }
              }
              else pos++;
         } break;
         case 3: //up
         {
              if (y>20)
              {
                 if(x==mx*20) //the case of one tile
                    {
                      if(y==my*20&&(map[my-1][mx]!=1&&map[my-1][mx]!=0))
                      {
                          if(rand()%2==0) pos=2;
                          else pos=4;
                      }
                    }
                 else if (y==my*20&&(map[my-1][mx]!=1&&map[my-1][mx]!=0)&&(map[my-1][mx+1]!=1&&map[my-1][mx+1]!=0))
                 {
                          if(rand()%2==0) pos=2;
                          else pos=4;
                 }
              }
              else pos=4;
         } break;
         case 2: //right
         {
              if (x+20<639)
              {
                 if(y==my*20) //the case of one tile
                    {
                      if(map[my][mx+1]!=1&&map[my][mx+1]!=0)
                      {
                          if(rand()%2==0) pos=1;
                          else pos=3;
                      }
                    }
                 else if ((map[my][mx+1]!=1&&map[my][mx+1]!=0)&&(map[my+1][mx+1]!=1&&map[my+1][mx+1]!=0))
                 {
                          if(rand()%2==0) pos=1;
                          else pos=3;
                 }
              }
              else pos++;
         } break;
         case 4: //left
         {
              if (x>20)
              {
                 if(y==my*20) //the case of one tile
                    {
                      if(x==mx*20&&(map[my][mx-1]!=1&&map[my][mx-1]!=0))
                      {
                          if(rand()%2==0) pos=1;
                          else pos=3;
                      }
                    }
                 else if (x==mx*20&&(map[my][mx-1]!=1&&map[my][mx-1]!=0)&&(map[my+1][mx-1]!=1&&map[my+1][mx-1]!=0))
                 {
                          if(rand()%2==0) pos=1;
                          else pos=3;
                 }
              }
              else pos=1;
         } break;
     }
     switch(pos)
     {
         case 0: break;
         case 1: //down
         {
               if (y+20<479) //checking borders
                 if(x==mx*20) //the case of one tile
                    {
                      if (y!=my*20) y++;
                      else if(map[my+1][mx]==1||map[my+1][mx]==0) y++;
                    }
                 else
                 {
                     if (y!=my*20) y++;
                     else if ((map[my+1][mx]==1||map[my+1][mx]==0)&&(map[my+1][mx+1]==1||map[my+1][mx+1]==0)) y++;
                 }
         } break;
         case 3: //up
         {
              if (y>0) //checking borders
                 if(x==mx*20) //the case of one tile
                    {
                      if (y!=my*20) y--;
                      else if(map[my-1][mx]==1||map[my-1][mx]==0) y--; 
                    }
                 else
                 {
                     if (y!=my*20) y--;
                     else if ((map[my-1][mx]==1||map[my-1][mx]==0)&&(map[my-1][mx+1]==1||map[my-1][mx+1]==0)) y--;
                 }
         } break;
         case 2: //right
         {
               if (x+20<639) //checking borders
                 if(y==my*20) //the case of one tile
                    {
                      if (x!=mx*20) x++;
                      else if(map[my][mx+1]==1||map[my][mx+1]==0) x++; 
                    }
                 else
                 {
                     if (x!=mx*20) x++;
                     else if ((map[my][mx+1]==1||map[my][mx+1]==0)&&(map[my+1][mx+1]==1||map[my+1][mx+1]==0)) x++;
                 }   
         } break;
         case 4: //left
         {
              if (x>0) //checking borders
                 if(y==my*20) //the case of one tile
                    {
                      if (x!=mx*20) x--;
                      else if(map[my][mx-1]==1||map[my][mx-1]==0) x--; 
                    }
                 else
                 {
                     if (x!=mx*20) x--;
                     else if ((map[my][mx-1]==1||map[my][mx-1]==0)&&(map[my+1][mx-1]==1||map[my+1][mx-1]==0)) x--;
                 }   
         } break;
     }
}

bool pacman::compareCoords(enemy monster)
{
     int monX, monY;
     monX=monster.get_x();
     monY=monster.get_y();
     if((monX>=x&&monX<x+20)&&(monY>=y&&monY<y+20)) return true;
     else return false;
}

void pacman::move(enemy mon[])
{
     int mx=getWhole(x/20);
     int my=getWhole(y/20);
     
     switch(pos)
     {
         case 0: break;
         case 1: //down
         {
               if (y+20<479) //checking borders
                 if(x==mx*20) //the case of one tile
                    {
                      if (y!=my*20) y++;
                      else if(map[my+1][mx]!=3) y++;
                      if (map[my+1][mx]==1) {map[my+1][mx]=0; points++;}
                    }
                 else
                 {
                     if (y!=my*20) y++;
                     else if ((map[my+1][mx]!=3)&&(map[my+1][mx+1]!=3)) y++;
                     if (map[my+1][mx]==1) {map[my+1][mx]=0; points++;}
                     else if(map[my+1][mx+1]==1) {map[my+1][mx+1]=0; points++;}
                 }
         } break;
         case 3: //up
         {
              if (y>0) //checking borders
                 if(x==mx*20) //the case of one tile
                    {
                      if (y!=my*20) y--;
                      else if(map[my-1][mx]!=3) y--; 
                      if(map[my][mx]==1) {map[my][mx]=0; points++;}
                    }
                 else
                 {
                     if (y!=my*20) y--;
                     else if ((map[my-1][mx]!=3)&&(map[my-1][mx+1]!=3)) y--;
                     if (map[my-1][mx]==1) {map[my-1][mx]=0; points++;}
                     else if (map[my-1][mx+1]==1) {map[my-1][mx+1]=0; points++;}
                 }
         } break;
         case 2: //right
         {
               if (x+20<639) //checking borders
                 if(y==my*20) //the case of one tile
                    {
                      if (x!=mx*20) x++;
                      else if(map[my][mx+1]!=3) x++;
                      if(map[my][mx+1]==1) {map[my][mx+1]=0; points++;}
                    }
                 else
                 {
                     if (x!=mx*20) x++;
                     else if ((map[my][mx+1]!=3)&&(map[my+1][mx+1]!=3)) x++;
                     if(map[my][mx+1]==1) {map[my][mx+1]=0; points++;}
                     else if(map[my+1][mx+1]==1) {map[my+1][mx+1]=0; points++;}
                 }   
         } break;
         case 4: //left
         {
              if (x>0) //checking borders
                 if(y==my*20) //the case of one tile
                    {
                      if (x!=mx*20) x--;
                      else if(map[my][mx-1]!=3) x--;
                      if(map[my][mx]==1) {map[my][mx]=0; points++;}
                    }
                 else
                 {
                     if (x!=mx*20) x--;
                     else if ((map[my][mx-1]!=3)&&(map[my+1][mx-1]!=3)) x--;
                     if (map[my][mx]==1) {map[my][mx]=0; points++;}
                     else if (map[my+1][mx]==1) {map[my+1][mx]=0; points++;}
                 }   
         } break;
     }
}

void pacman::setImage()
{
     if(pos==0||time%20>10) man = load_bitmap("pacman.bmp",NULL);
     else if(pos==1) man = load_bitmap("pacman_down.bmp",NULL);
     else if(pos==2) man = load_bitmap("pacman_right.bmp",NULL);
     else if(pos==3) man = load_bitmap("pacman_up.bmp",NULL);
     else if(pos==4) man = load_bitmap("pacman_left.bmp",NULL);
}

void init();
void deinit();

int main() 
{
    int x,y;
    enemy mon[4];
    mon[0].set(40,200);
    mon[1].set(300,40);
    mon[2].set(300,300);
    mon[3].set(20,200);
    
	init();
	buffer = create_bitmap(640,480);
    point=load_bitmap("point.bmp", NULL);
    beep=load_sample("beep.wav");
    int pos=0;
    player.time=0;
    int timeChange;
    
	while (!key[KEY_ESC]&&!player.dead) //game loop
    {
//plane drawing
        
        rectfill(buffer,0,0,640,480,makecol(0,0,0));
        for (int i=0; i<=24; i++)
        {
                for(int t=0; t<=32; t++)
                {
                     if(map[i][t]==1) draw_sprite(buffer, point, t*20, i*20);
                     if(map[i][t]==2) rectfill(buffer, t*20, i*20, (t+1)*20, (i+1)*20, makecol(100,0,100));
                     else if(map[i][t]==3) rectfill(buffer, t*20, i*20, (t+1)*20, (i+1)*20, makecol(100,0,100));
                     else if( map[i][t]==4) rectfill(buffer, t*20, i*20, (t+1)*20, (i+1)*20, makecol(0,0,255));
                }       
        }

//player's move 
        if(key[KEY_UP]) player.pos=3;
        else if (key[KEY_DOWN]) player.pos=1;
        else if (key[KEY_RIGHT]) player.pos=2;
        else if (key[KEY_LEFT]) player.pos=4;
        else if (key[KEY_SPACE]) player.pos=0;
        int howManyPoints=player.points;
        
        player.move(mon);
        player.setImage();
        player.time++;
        if(player.points>howManyPoints) play_sample(beep, 255, 128, 1000, false);
        
        x=player.get_x();
        y=player.get_y();
        draw_sprite(buffer, man, x, y);
        for(int i=0; i<4; i++)
        {
                mon[i].enMove();
                if(player.compareCoords(mon[i])) {player.dead=true;}
                if(i==0) monster = load_bitmap("mon.bmp",NULL);
                else if(i==1) monster = load_bitmap("mon2.bmp",NULL);
                else if(i==2) monster = load_bitmap("mon3.bmp",NULL);
                else if(i==3) monster = load_bitmap("mon4.bmp",NULL);
                draw_sprite(buffer, monster, mon[i].get_x(), mon[i].get_y());
        }
        char point[10];
        sprintf(point, "%d points", player.points);
        textout_ex(buffer,font,point,500,0,makecol(255,0,0),makecol(0,0,0));
        draw_sprite(screen, buffer, 0, 0);
        rest(30);
	}
	BITMAP* end;
	end=load_bitmap("GAMEOVER.bmp", NULL);
	draw_sprite(screen, end, 0, 0);
	while(!key[KEY_ENTER]);
	deinit();
	return 0;
}
END_OF_MAIN();

void init() 
{
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	set_volume(255, 255);
	install_timer();
	install_keyboard();
	install_mouse();
	/* add other initializations here */
}

void deinit() 
{
	clear_keybuf();
	/* add other deinitializations here */
}
