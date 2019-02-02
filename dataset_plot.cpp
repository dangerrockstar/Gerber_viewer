#include "dataset_plot.h"
#include "mainwindow.h"
#include<opencv.hpp>
#include<opencv_modules.hpp>


void dataset_plot::arc_refined_new(bool cw,double x1, double y1, double x2, double y2, double c1, double c2,vector<cv::Point> &vertices)
{
    vertices.clear();
    double temp_x;
    double r=sqrt(pow(((int)x1-(int)c1),2)+pow(((int)y1-(int)c2),2));


    if(cw)
    {
         vertices.push_back(cv::Point2f((x1),(y1)));
         if(((x1==x2)&&(y1==y2)) && ((x1 >= c1) && (x2>= c1)))
         {
             for(int a=y1;a>c2-r;a--)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 +(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }
             for(int a=c2-r;a<c2+r;a++)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 -(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }
             for(int a=c2+r;a>y1;a--)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 +(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }
          }
         else if(((x1==x2)&&(y1==y2)) && ((x1 <= c1) && (x2<= c1)))
         {
             for(int a=y1;a<c2+r;a++)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 -(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }
             for(int a=c2+r;a>c2-r;a--)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 +(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }
             for(int a=c2-r;a<y1;a++)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 -(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }

         }


        else if((x1==c1)&&(x2==c1))
        {
            if((y1<c2)&&(c2<y2))
            {
                for(int a=y1;a<y2;a++)
                {
                    if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                    {
                        temp_x=c1 -(sqrt(pow(r,2)-pow((a-c2),2)));
                        vertices.push_back(cv::Point2f((temp_x),a));
                    }
                }
                vertices.push_back(cv::Point2f((x2),(y2)));
            }
            else if((y1>c2)&&(c2>y2))
            {
                for(int a=y1;a>y2;a--)
                {
                    if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                    {
                        temp_x=c1 + (sqrt(pow(r,2)-pow((a-c2),2)));
                        vertices.push_back(cv::Point2f((temp_x),a));
                    }
                }
                vertices.push_back(cv::Point2f((x2),(y2)));
            }
        }
           else if ((x1 >= c1) && (x2>= c1))
            {
                if(y1<y2)
                    {

                    for(int a=y1;a>c2-r;a--)
                    {
                        if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                        {
                            temp_x=c1 +(sqrt(pow(r,2)-pow((a-c2),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                        }
                    }
                    for(int a=c2-r;a<c2+r;a++)
                    {
                        if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                        {
                            temp_x=c1 -(sqrt(pow(r,2)-pow((a-c2),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                        }
                    }
                    for(int a=c2+r;a>y2;a--)
                    {
                        if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                        {
                            temp_x=c1 +(sqrt(pow(r,2)-pow((a-c2),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                        }
                    }
                    vertices.push_back(cv::Point2f((x2),(y2)));
                }
                if(y1>y2)
                {
                    for(int a=y1;a>y2;a--)
                    {
                        if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                        {
                            temp_x=c1 +(sqrt(pow(r,2)-pow((a-c2),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                        }
                    }
                    vertices.push_back(cv::Point2f((x2),(y2)));
                 }

            }
           else if ((x1 <= c1) && (x2 <= c1))
           {
                if(y1>y2)
                {

                    for(int a=y1;a<c2+r;a++)
                    {
                        if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                        {
                            temp_x=c1 -(sqrt(pow(r,2)-pow((a-c2),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                        }
                    }
                    for(int a=c2+r;a>c2-r;a--)
                    {
                        if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                        {
                            temp_x=c1+(sqrt(pow(r,2)-pow((a-c2),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                        }
                    }
                    for(int a=c2-r;a<y2;a++)
                    {
                        if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                        {
                            temp_x=c1-(sqrt(pow(r,2)-pow((a-c2),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                        }
                    }
                    vertices.push_back(cv::Point2f((x2),(y2)));
                }
                if(y1<y2)
                {
                    for(int a=y1;a<y2;a++)
                    {
                        if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                        {
                            temp_x=c1 - (sqrt(pow(r,2)-pow((c2-a),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                        }
                    }
                    vertices.push_back(cv::Point2f((x2),(y2)));
                 }

           }
           else if ((x1 >= c1) && (x2 <= c1))
           {
               for(int a=y1;a>c2 -r;a--)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1 + (sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                    }
               }
               for(int a=c2-r;a<y2;a++)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1 - (sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                    }
               }
            vertices.push_back(cv::Point2f((x2),(y2)));
           }
           else if ((x1 <= c1) && (x2 >= c1))
           {
               for(int a=y1;a <c2+r;a++)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1 - (sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                   }
               }
               for(int a=c2+r;a>y2;a--)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1 +(sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                   }
               }
               vertices.push_back(cv::Point2f((x2),(y2)));
            }


     }
    else if(cw==0)
    {
         vertices.push_back(cv::Point2f((x1),(y1)));

         if(((x1==x2)&&(y1==y2)) && ((x1 >= c1) && (x2>= c1)))
         {
             for(int a=y1;a<c2+r;a++)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 +(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }
             for(int a=c2+r;a>c2-r;a--)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 -(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }
             for(int a=c2-r;a<y1;a++)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 +(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }
          }
         else if(((x1==x2)&&(y1==y2)) && ((x1 <= c1) && (x2<= c1)))
         {
             for(int a=y1;a>c2-r;a--)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 -(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }
             for(int a=c2-r;a<c2+r;a++)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 +(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }
             for(int a=c2+r;a>y1;a--)
             {
                 if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                 {
                     temp_x=c1 -(sqrt(pow(r,2)-pow((a-c2),2)));
                     vertices.push_back(cv::Point2f((temp_x),a));
                 }
             }

         }



       else if((x1==c1)&&(x2==c1))
        {
            if(y1<y2)
            {
                for(int a=y1;a<y2;a++)
                {
                    if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                    {
                        temp_x=c1+(sqrt(pow(r,2)-pow((a-c2),2)));
                        vertices.push_back(cv::Point2f((temp_x),a));
                    }
                }
                vertices.push_back(cv::Point2f((x2),(y2)));
            }
            else if(y1>y2)
            {
                for(int a=y1;a>y2;a--)
                {
                    if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                    {
                        temp_x=c1-(sqrt(pow(r,2)-pow((a-c2),2)));
                        vertices.push_back(cv::Point2f((temp_x),a));
                    }
                }
               vertices.push_back(cv::Point2f((x2),(y2)));
            }
        }
       else if ((x1 >= c1) && (x2>= c1))
        {
           if(y1>y2)
           {

               for(int a=y1;a<c2+r;a++)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1+(sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                    }
               }
               for(int a=c2+r;a>c2-r;a--)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1-(sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                    }
               }
               for(int a=c2-r;a<y2;a++)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1+(sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                    }
               }
               vertices.push_back(cv::Point2f((x2),(y2)));

           }
           if(y1<y2)
           {
               for(int a=(y1);a<y2;a++)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1+(sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                    }
               }
               vertices.push_back(cv::Point2f((x2),(y2)));
           }

        }
           else if ((x1 <= c1) && (x2 <= c1))
           {
                if(y1<y2)
                {
                    //cout<<x1<<"," <<y1<<",,,"<<x2 <<","<<y2<<endl;
                    for(int a=y1;a>c2-r;a--)
                    {
                        if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                        {
                            temp_x=c1-(sqrt(pow(r,2)-pow((a-c2),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                         }
                    }
                    for(int a=c2-r;a<c2+r;a++)
                    {
                        if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                        {
                            temp_x=c1+(sqrt(pow(r,2)-pow((a-c2),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                         }
                    }
                    for(int a=c2+r;a>y2;a--)
                    {
                        if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                        {
                            temp_x=c1-(sqrt(pow(r,2)-pow((a-c2),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                         }
                    }
                    vertices.push_back(cv::Point2f((x2),(y2)));
                }
                if(y1>y2)
                {
                    for(int a=y1;a>y2;a--)
                    {
                        if(sqrt(pow(r,2)-pow((c2-a),2))>=0)
                        {
                            temp_x=c1 - (sqrt(pow(r,2)-pow((a-c2),2)));
                            vertices.push_back(cv::Point2f((temp_x),a));
                        }
                    }
                    vertices.push_back(cv::Point2f((x2),(y2)));
                }

           }
           else if( (x1 >= c1) && (x2 <= c1))
           {
               for(int a=y1;a<c2 +r;a++)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1 + (sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                   }
               }
               for(int a=c2+r; a > y2; a--)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1 - (sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                   }
               }
               vertices.push_back(cv::Point((x2),(y2)));
           }
           else if( (x1 <= c1) && (x2 >= c1))
           {
               for(int a=y1;a > c2-r;a--)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1 - (sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                   }
               }
               for(int a=c2-r;a<y2;a++)
               {
                   if(sqrt(pow(r,2)-pow((a-c2),2))>=0)
                   {
                       temp_x=c1 + (sqrt(pow(r,2)-pow((a-c2),2)));
                       vertices.push_back(cv::Point2f((temp_x),a));
                   }
               }
               vertices.push_back(cv::Point2f((x2),(y2)));
            }
      }
}




//_______________________________________________________________________________________________//
    void dataset_plot::arc_new(cv::Mat&surface ,double x1, double y1, double x2, double y2, double c1, double c2,bool flag , vector<cv::Point> &vertices)
{
        vertices.clear();

        float PI = 3.14159265;
        float c,ca,cb,cc;
        int angle,s_angle,e_angle,start_angle,end_angle;
        float dot=((x1-c1)*(x2-c1))+((y1-c2)*(y2-c2));
        float r=sqrt(pow((x1-c1),2)+pow((y1-c2),2));
        float mag_1=sqrt(pow((x1-c1),2)+pow((y1-c2),2));
        float mag_2=sqrt(pow((x2-c1),2)+pow((y2-c2),2));
        if(((mag_1*mag_2)==0)&&(dot))
            c=1.0;
        else
            c=dot/(mag_1*mag_2);
        angle=acos(c)*(180.0)/PI;

        float axis_x,axis_y;
        axis_x=c1+r, axis_y=c2;

        float dot1=((x1-c1)*(axis_x-c1))+((y1-c2)*(axis_y-c2));
        float mag_11=sqrt(pow((x1-c1),2)+pow((y1-c2),2));
        float mag_21=sqrt(pow((axis_x-c1),2)+pow((axis_y-c2),2));
        if(((mag_11*mag_21)==0)&&(dot1))
             ca=1.0;
        else if(!dot1)
             ca=0.0;
        else
             ca=dot1/(mag_11*mag_21);
        s_angle=acos(ca)*(180.0)/PI; //angle of start point with 3 o'clock

        float dot2=((x2-c1)*(axis_x-c1))+((y2-c2)*(axis_y-c2));
        float mag_12=sqrt(pow((x2-c1),2)+pow((y2-c2),2));
        float mag_22=sqrt(pow((axis_x-c1),2)+pow((axis_y-c2),2));
        if(((mag_12*mag_22)==0)&&(dot2))
             cb=1.0;
        else if(!dot2)
             cb=0.0;
        else
             cb=dot2/(mag_12*mag_22);
        e_angle=acos(cb)*(180.0)/PI;

       if(y1<c2)
           start_angle=-s_angle;
       else
           start_angle=s_angle;

       if(y2<c2)
           end_angle=-e_angle;
       else
           end_angle=e_angle;

        float a1 = start_angle*(PI/180.0);
        float a2 = end_angle*(PI/180.0);
        cv::Point arc_c;
        arc_c.x = c1;
        arc_c.y= c2;
        vertices.push_back(arc_c);
         vector<vector<cv::Point>> temp_vertices;
         temp_vertices.push_back(vertices);

        if(flag==0)
        {
            cv::drawContours(surface , temp_vertices , 0 , cv::Scalar(255) , 8 , cv::LINE_8  );
        }
        else
        {
            cv::drawContours(surface , temp_vertices , 0 , cv::Scalar(255) , 8 , cv::LINE_8  );
        }

}

