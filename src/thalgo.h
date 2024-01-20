// Copyright 2024 Nash Sun.
// Copyright 2024 dynamder.


//以下代码还未经过测试
struct Pair
{
    float x,y;
};

struct Dot
{
    Pair pos,v,a;
    float r;//判定范围半径
};
struct Line
{
    Pair hpos,tpos;//头尾坐标
    Pair hv,tv;//头尾速度
    Pair ha,ta;//头尾加速度
    float r;//判定线宽(一半)
};
struct Curve//匀加速曲线运动的轨迹曲线描述
{
    Pair spos;
    Pair v,a;
    float st,et;//start time and end time
    float r;//同前
};
void v_interpolate()//占个位，参数返回值待定

float d2d_sqdist(Dot a, Dot b)//点到点距离的平方
{
    float dx=a.pos.x-b.pos.x;
    float dy=a.pos.y-b.pos.y;
    return dx*dx+dy*dy;
}
float d2d_dist(Dot a, Dot b)
{
    return sqrt(d2d_sqdist(a,b));
}

bool d2dcollic(Dot a, Dot b)//静态判定，圆正交
{
    if(d2d_sqdist(a,b)>=a.r*a.r+b.r*b.r)
    {
        return 0;
    }else return 1;
}
bool d2dcollic_d(Dot a, Dot b,bool interpolation=0)//动态判定，圆正交(高速防神穿)
{
    if(interpolation==0)
    {
        if(a.v.x==b.v.x&&a.v.y==b.v.y)
        {
            return d2dcollic(a,b);
        }else
        {
            float dx=a.pos.x-b.pos.x;
            float dy=a.pos.y-b.pos.y;
            float dvx=a.v.x-b.v.x;
            float dvy=a.v.y-b.v.y;
            float mod_dv=(dvx*dvx+dvy*dvy);
            float t=-1*(dx*dvx+dy*dvy);
            float sdmin=0;
            if(t<0) sdmin=dx*dx+dy*dy;
            else if(t>mod_dv) sdmin=(dx+dvx)*(dx+dvx)+(dy+dvy)*(dy+dvy);
            else if(0<=t&&t<=mod_dv) return 1;

            if(sdmin>=a.r*a.r+b.r*b.r) return 0;
            else return 1;
        }

    }else
    {
        v_interpolate();
        //还没写
        return 0;
    }

}

bool d2dcollis(Dot a,Dot b)//静态判定，方形相交
{
    float dx=a.pos.x-b.pos.x;
    float dy=a.pos.y-b.pos.y;
    if((dx*2>=a.r+b.r||dx*2<=-a.r-b.r)&&(dy*2>=a.r+b.r||dy*2<=-a.r-b.r))
    {
        return 0;
    }else return 1;
}
bool d2dcollis_d(Dot a,Dot b,bool interpolation=0)//动态判定，方形相交
{
    if(interpolation==0)
    {
        if(a.v.x==b.v.x&&a.v.y==b.v.y)
        {
            return d2dcollis(a,b);
        }else
        {
            float dx=a.pos.x-b.pos.x;
            float dy=a.pos.y-b.pos.y;
            float dvx=a.v.x-b.v.x;
            float dvy=a.v.y-b.v.y;

            float sdxmin=0;
            float sdymin=0;
            if(dx>0)
            {
                sdxmin=dx*dx;
            }
            else if(dx<-1*dvx) sdxmin=(dx+dvx)*(dx+dvx);
            else if(-1*dvx<=dx&&dx<=0) return 1;

            if(dy>0)
            {
                sdymin=dy*dy;
            }
            else if(dy<-1*dvy) sdymin=(dy+dvy)*(dy+dvy);
            else if(-1*dvy<=dy&&dy<=0) return 1;

            if(2*sdxmin>=a.r+b.r&&2*sdymin>=a.r+b.r) return 0;
            else return 1;
        }

    }else
    {
        v_interpolate();
        //还没写
        return 0;
    }

}
bool l2dcolli(Line a, Dot b)//直线激光与点判定，仅静态,内接椭圆法
{
    float dx=a.tpos.x-a.hpos.x;
    float dy=a.tpos.y-a.hpos.y;

    float midx=(a.hpos.x+a.tpos.x)/2;
    float midy=(a.hpos.y+a.tpos.y)/2;

    float sdis=dx*dx+dy*dy;
    float dis=sqrt(sdis);

    float c=sqrt(sdis/4-a.r*a.r);
    float fx1=midx+c*dx/dis;
    float fy1=midy+c*dy/dis;
    float fx2=midx-c*dx/dis;
    float fy2=midy-c*dy/dis;

    float p1=sqrt((b.pos.x-fx1)*(b.pos.x-fx1)+(b.pos.y-fy1)*(b.pos.y-fy1));
    float p2=sqrt((b.pos.x-fx2)*(b.pos.x-fx2)+(b.pos.y-fy2)*(b.pos.y-fy2));

    if(p1+p2>=dis) return 0;
    else return 1;

}
