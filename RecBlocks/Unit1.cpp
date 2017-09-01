//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

class Block
{
public:
char color;
int x,y;
int px,py;
char selection;
};

Block*blocks[640];
int score=0,tempscore=0;
Graphics::TBitmap*BackBuffer;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
randomize();
BackBuffer=new Graphics::TBitmap();
BackBuffer->Width=640 ;
BackBuffer->Height=400;
int Count=0;
for(int j=0;j<20;j++)
{
        for(int i=0;i<32;i++,Count++)
        {
        blocks[Count]=new Block();
        blocks[Count]->x=20*i;
        blocks[Count]->y=20*j;
        blocks[Count]->px=i;
        blocks[Count]->py=j;
        blocks[Count]->color=random(4);
        blocks[Count]->selection=0;
        }
}
}
//---------------------------------------------------------------------------


Block*GetXYBlock(int x,int y)
{
for(int i=0;i<640;i++)
	{
	if((blocks[i]->x<=x)&&((blocks[i]->x+20)>x)&&(blocks[i]->y<=y)&&((blocks[i]->y+20)>y))
		{
		return blocks[i];
		}
	}
return NULL;
}
void DeselectBlocks()
	{
	for(int i=0;i<640;i++)
		{
		blocks[i]->selection=0;
		}
	}

Block*GetBlock(int x,int y)
{
for(int i=0;i<640;i++)
	{
	if((blocks[i]->px==x)&&(blocks[i]->py==y))
		{
		return blocks[i];
		}
	}
return NULL;
}

int TestNeighbour(int x,int y,char c)
{
Block*b=GetBlock(x,y);
if(b!=NULL)
{
if(b->color==c)
	{
	b->color=6;
	int n=1;
	n+=TestNeighbour(x-1,y,c);
	n+=TestNeighbour(x+1,y,c);
	n+=TestNeighbour(x,y-1,c);
	n+=TestNeighbour(x,y+1,c);
	return n;
	}else
	{
	return 0;
	}
}
return 0;
}

void CollapseDown()
{
Block*b;
for(int x=0;x<32;x++)
	for(int y=19;y>0;y--)
	{
	b=GetBlock(x,y);
	char thisColor=b->color;
	if(thisColor==6)
		{
		for(int i=y-1;i>=0;i--)
			{
			b=GetBlock(x,i);
			char aboveColor=b->color;
			if(aboveColor!=6)
				{
				b=GetBlock(x,y);
				b->color=aboveColor;
				b=GetBlock(x,i);
				b->color=6;
				break;
				}
			}
		}
	}
}

void CollapseAcross()
{
int n;
Block*b;
	do
	{
	n=0;
	for(int x=0;x<32;x++)
		{
		b=GetBlock(x,19);
                if(b!=NULL)
                {
		if(b->color==6)
			{
			b=GetBlock(x+1,19);
                        if(b!=NULL)
                        {
			if(b->color!=6)
				{
				n++;

				for(int y=0;y<20;y++)
					{
					b=GetBlock(x+1,y);
                                        if(b!=NULL)
                                        {
					char c=b->color;
					b->color=6;
					b=GetBlock(x,y);
					b->color=c;
                                        }
					}
				}
                        }
			}
                }
		}
	}while(n>0);
}
int SelectTestNeighbour(int x,int y,char c)
{
Block*b=GetBlock(x,y);
if(b!=NULL)
{
if(b->color==c)
	{
	if(b->selection!=1)
	{
	b->selection=1;
	int n=1;
	n+=SelectTestNeighbour(x-1,y,c);
	n+=SelectTestNeighbour(x+1,y,c);
	n+=SelectTestNeighbour(x,y-1,c);
	n+=SelectTestNeighbour(x,y+1,c);
	return n;
	}
	}else
	{
	return 0;
	}
}
return 0;
}
void SelectBlock(int Mx,int My)
{
Block*b=GetXYBlock(Mx,My);
if(b!=NULL)
{
if(b->color!=6)
{
if(b->selection!=1)
{
char tempcolor=b->color;
b->selection=1;
int x=b->px;
int y=b->py;
int n=1;
n+=SelectTestNeighbour(x-1,y,tempcolor);
n+=SelectTestNeighbour(x+1,y,tempcolor);
n+=SelectTestNeighbour(x,y-1,tempcolor);
n+=SelectTestNeighbour(x,y+1,tempcolor);
if(n>1)
	{
	tempscore=n*n;
	}else
	{
	b->selection=0;
	tempscore=0;
	}
}
}
}
}

void ClickBlock(int mx,int my)
{
Block*b=GetXYBlock(mx,my);
if(b->color!=6)
{
char tempcolor=b->color;
b->color=6;
int x=b->px;
int y=b->py;
int n=1;
n+=TestNeighbour(x-1,y,tempcolor);
n+=TestNeighbour(x+1,y,tempcolor);
n+=TestNeighbour(x,y-1,tempcolor);
n+=TestNeighbour(x,y+1,tempcolor);
if(n>1)
	{
  	CollapseDown();
  	CollapseAcross();
	score+=n*n;
	}else
	{
	b->color=tempcolor;
	}
}
}


TForm1::DrawAllBlocks()
{
  for(int i=0;i<640;i++)
        {
        switch(blocks[i]->color)
                {
                case 0:BackBuffer->Canvas->Brush->Color=clRed;break;
                case 1:BackBuffer->Canvas->Brush->Color=clGreen;break;
                case 2:BackBuffer->Canvas->Brush->Color=clBlue;break;
                case 3:BackBuffer->Canvas->Brush->Color=clPurple;break;
                case 6:BackBuffer->Canvas->Brush->Color=clBlack;break;
                }
                if(!blocks[i]->selection)
                        BackBuffer->Canvas->FillRect(Rect(blocks[i]->x,blocks[i]->y,blocks[i]->x+30,blocks[i]->y+30));
                else
                {
                BackBuffer->Canvas->Brush->Color=clGray;
                BackBuffer->Canvas->FillRect(Rect(blocks[i]->x,blocks[i]->y,blocks[i]->x+30,blocks[i]->y+30));
                }
        }
return 0;
}
void __fastcall TForm1::FormPaint(TObject *Sender)
{
DrawAllBlocks();
Flip();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
ClickBlock(X,Y);
DrawAllBlocks();
Flip();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
DeselectBlocks();
tempscore=0;
TPoint p=Form1->ScreenToClient(Mouse->CursorPos);
SelectBlock(p.x,p.y);
DrawAllBlocks();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
DeselectBlocks();
tempscore=0;
TPoint p=Form1->ScreenToClient(Mouse->CursorPos);
SelectBlock(p.x,p.y);
DrawAllBlocks();
Flip();
Caption="Блоки      Очки:"+IntToStr(score)+"     Очки за блок:"+IntToStr(tempscore);
}
//---------------------------------------------------------------------------


void TForm1::Flip()
{
        Canvas->Draw(0,0,BackBuffer);
}
