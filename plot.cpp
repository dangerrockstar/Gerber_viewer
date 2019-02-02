#include "plot.h"
#include "dataset_plot.h"
#include<opencv.hpp>
#include<opencv_modules.hpp>
#include"mainwindow.h"

void plot:: plot_gerber(std::string file_path , cv::Mat&surface )
{
    // >>>>>>>>>>>>>>>>>>>file reading section  >>>>>>>>>>>>>>>>>>>>>>>>>>>>//
    std::string file="symbol.txt";
    std::string ss= "cd "+ file_path + " && ls >" + file;
    std::system(ss.c_str());
    std::ifstream file_layer(file_path+file);
    std::ifstream file_draw_layer(file_path+file);
    // >>>>>>>>>>>>>>>>>>>>>>>>>>end file reading section>>>>>>>>>>>>>>>>>>>>>>>>  //

    // >>>>>>>>>>>>>>>>>>>>>global variables declarations section  >>>>>>>>>>>>>>>>>>>>>//
    std::vector<cv::Point> cordinate;
    vector<vector<cv::Point>> fill ;
    std ::string Xorigin ,Yorigin , Iorigin, Jorigin ,Dcommand , line,file_line , D_code_key;
    float y_core =0 ,x_core =0 , i_core , j_core ;
    string x_dec,y_dec,x_frac,y_frac,zero="0",x_point,y_point, i_point, j_point;
    int negativ_value=  0;

    vector<cv::Point> vertices;
     vector<vector<cv::Point>> unshap_vertices;


    // >>>>>>>>>>>>>>>>>>>>>end variable declarations section >>>>>>>>>>>>>>>>>>>>>>>>>//


// >>>>>>>>>>>>>>>>>>>>>>Line reading section>>>>>>>>>>>>>>>>>>>>>>>>>>>> //
while(getline(file_draw_layer,file_line))
{
    float xd,xf,yd,yf;
    if (file_line !="symbol.txt" && file_line == "gts" )
//                if (file_line !="symbol.txt"  )
    {
            //  >>>>>>>>>>>>>>flag section variable>>>>>>>>>>>>>>>//
            int g_75=0 , g_36=0 , g_01=0 , g_02=0 , d10=0 , g_37=0 , g_54 = 0 , lpd=0 , lpc=0;
            // >>>>>>>>>>>>>end flag section >>>>>>>>>>>>>>>>>>>>>>>//

            float frac=0.0 ;
            map <string , char > symbol;
            std::vector<float>  dcode_value;
            map<string,vector<float>>D_symbol_code;

            std::ifstream file(file_path+file_line);
            while(std::getline(file, line))
            {
//                 qDebug()<<"Line"<<QString::fromStdString(line)<<"\t";

                int flag_i=0 , flag_j = 0 ,flag_y = 0 ,flag_x = 0 ;
// >>>>>>>>>>>>>>>>start find X,Y,I,J,D >>>>>>>>>>>>>>>>//

                int  indexY =-1 , indexX =-1  ,indexD= -1,indexG=-1 ,indexI =-1 , indexJ = -1 ;
                indexD = line.find('D');
                indexY= line.find('Y');
                indexX= line.find('X');
                indexG= line.find('G');
                indexI = line.find('I');
                indexJ = line.find('J');

// >>>>>>>>>>>>find part End >>>>>>>>>>>>>>>>>>>>//

//***************File End condition*****************/
                if(line=="M02*")
                {
                    continue;
                }
//***************File End condition*****************/

// >>>>>>>>>>>>> Start % section  start >>>>>>>>>>>>>>>>//
                if(line.at(0) =='%')
                {
                    if(line.at(1) =='F')
                    {
                        // >>>>faction value >>>>//
                        Xorigin = line.substr(indexX+1,(indexY-indexX-1));
                        Yorigin = line.substr(indexY+1,(indexY-indexX-1));
                        int x_temp = atof(Xorigin.c_str()) ;
                        int  y_temp = (atof(Yorigin.c_str())) ;
                        xf = x_temp % 10;
                        xd = x_temp  / 10 ;
                        yf = y_temp % 10 ;
                        yd = y_temp  / 10 ;
                        //>>>>>>>>>>>End >>>>>>>>>>.//
                    }


                    if(line.at(1) =='A' && line.at(2) == 'D')
                    {
                        string line2, line3 , line4, line5, line6, line7;
                        indexD = line.find('*');
                        indexY = line.find(',');
                        indexX = line.find('X');
                        string decode= line.substr(3+1,indexY-4);

                        if( line.at(indexY-1)  == 'C')
                        {
                            line2 = line.substr((indexY-4) , indexY-3);
                            line3 = line.substr(indexY+1 , indexD-indexY-1);
                            line4 = line3.substr(0 , (line3.find('X')-1));
                            dcode_value.push_back((atof(line4.c_str()))/FOutputBmpMilPerPxl);

                            std::size_t found = line3.find('X');
                             if(found != std::string::npos )
                            {
                                 line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                                 line5 = line3.substr(0,(line3.find('X')-1));
                                 dcode_value.push_back((atof(line5.c_str()))/FOutputBmpMilPerPxl);

                            }
                             else
                             {
                                 line3.erase();
                             }
                            D_symbol_code.insert(std::pair<string , vector<float>>( line2,dcode_value ));
                            symbol.insert(std::pair<string ,char >( line.substr((indexY-4) , indexY-4),line2[3]));
                            dcode_value.clear();
                            line3.erase();
                        }
                        else if(line.at(indexY-1)  == 'R')
                        {
                            line2 = line.substr((indexY-4) , indexY-3);
                            line3 = line.substr(indexY+1 , indexD-indexY-1);
                            line4 = line3.substr(0 , (line3.find('X')-1));
                            line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                            line5 = line3.substr(0,(line3.find('X')-1));
                            dcode_value.push_back((atof(line4.c_str()))/FOutputBmpMilPerPxl);
                            dcode_value.push_back((atof(line5.c_str()))/FOutputBmpMilPerPxl);

                            std::size_t found = line3.find('X');
                             if(found != std::string::npos )
                            {
                                 line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                                 line6 = line5 = line3.substr(0,(line3.find('X')-1));
                                 dcode_value.push_back((atof(line6.c_str()))/FOutputBmpMilPerPxl);
                            }
                             else
                             {
                                 line3.erase();
                             }

                            D_symbol_code.insert(std::pair<string , vector<float>>( line2,dcode_value ));
                            symbol.insert(std::pair<string ,char >( line.substr((indexY-4) , indexY-4),line2[3]));
                            dcode_value.clear();
                            line3.erase();
                        }
                        else if(line.at(indexY-1) == 'O')
                        {
                            line2 = line.substr((indexY-4) , indexY-3);
                            line3 = line.substr(indexY+1 , indexD-indexY-1);
                            line4 = line3.substr(0 , (line3.find('X')-1));
                            line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                            line5 = line3.substr(0,(line3.find('X')-1));
                            dcode_value.push_back((atof(line4.c_str()))/FOutputBmpMilPerPxl);
                            dcode_value.push_back((atof(line5.c_str()))/FOutputBmpMilPerPxl);

                            std::size_t found = line3.find('X');
                             if(found != std::string::npos )
                            {
                                 line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                                 line6 = line5 = line3.substr(0,(line3.find('X')-1));
                                 dcode_value.push_back((atof(line6.c_str()))/FOutputBmpMilPerPxl);
                            }
                             else
                             {
                                 line3.erase();
                             }

                            D_symbol_code.insert(std::pair<string , vector<float>>( line2,dcode_value ));
                            symbol.insert(std::pair<string ,char >( line.substr((indexY-4) , indexY-4),line2[3]));
                            dcode_value.clear();
                            line3.erase();

                        }
                        else if(line.at(indexY-1) == 'P')
                        {
                            line2 = line.substr((indexY-4) , indexY-3);
                            line3 = line.substr(indexY+1 , indexD-indexY-1);
                            line4 = line3.substr(0 , (line3.find('X')-1));
                            line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                            line5 = line3.substr(0,(line3.find('X')-1));
                            dcode_value.push_back((atof(line4.c_str()))/FOutputBmpMilPerPxl);
                            dcode_value.push_back((atof(line5.c_str()))/FOutputBmpMilPerPxl);

                            std::size_t found = line3.find('X');
                             if(found != std::string::npos )
                            {
                                 line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                                 line6 = line5 = line3.substr(0,(line3.find('X')-1));
                                 line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                                 line7 = line3.substr(0,(line3.find('X')-1));
                                 dcode_value.push_back((atof(line6.c_str()))/FOutputBmpMilPerPxl);
                                 dcode_value.push_back((atof(line7.c_str()))/FOutputBmpMilPerPxl);
                            }
                             else
                             {
                                 line3.erase();
                             }

                            D_symbol_code.insert(std::pair<string , vector<float>>( line2,dcode_value ));
                            symbol.insert(std::pair<string ,char >( line.substr((indexY-4) , indexY-4),line2[3]));
                            dcode_value.clear();
                            line3.erase();
                        }
                    }
                    else if(line.at(1) =='A' && line.at(2) == 'M')
                    {

                        string line2, line3 , line4, line5, line6, line7;
                        indexD = line.find('*');
                        indexY = line.find(',');
                        indexX = line.find('X');
                        string decode= line.substr(3+1,indexY-4);

                        if( line.at(indexY-1)  == 'C')
                        {
                            line2 = line.substr((indexY-4) , indexY-3);
                            line3 = line.substr(indexY+1 , indexD-indexY-1);
                            line4 = line3.substr(0 , (line3.find('X')-1));
                            dcode_value.push_back((atof(line4.c_str()))/FOutputBmpMilPerPxl);

                            std::size_t found = line3.find('X');
                             if(found != std::string::npos )
                            {
                                 line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                                 line5 = line3.substr(0,(line3.find('X')-1));
                                 dcode_value.push_back((atof(line5.c_str()))/FOutputBmpMilPerPxl);

                            }
                             else
                             {
                                 line3.erase();

                             }
                            D_symbol_code.insert(std::pair<string , vector<float>>( line2,dcode_value ));
                            symbol.insert(std::pair<string ,char >( line.substr((indexY-4) , indexY-4),line2[3]));
                            dcode_value.clear();
                            line3.erase();
                        }
                        else if(line.at(indexY-1)  == 'R')
                        {
                            line2 = line.substr((indexY-4) , indexY-3);
                            line3 = line.substr(indexY+1 , indexD-indexY-1);
                            line4 = line3.substr(0 , (line3.find('X')-1));
                            line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                            line5 = line3.substr(0,(line3.find('X')-1));
                            dcode_value.push_back((atof(line4.c_str()))/FOutputBmpMilPerPxl);
                            dcode_value.push_back((atof(line5.c_str()))/FOutputBmpMilPerPxl);

                            std::size_t found = line3.find('X');
                             if(found != std::string::npos )
                            {
                                 line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                                 line6 = line5 = line3.substr(0,(line3.find('X')-1));
                                 dcode_value.push_back((atof(line6.c_str())));
                            }
                             else
                             {
                                 line3.erase();
                             }

                            D_symbol_code.insert(std::pair<string , vector<float>>( line2,dcode_value ));
                            symbol.insert(std::pair<string ,char >( line.substr((indexY-4) , indexY-4),line2[3]));
                            dcode_value.clear();
                            line3.erase();
                        }
                        else if(line.at(indexY-1) == 'O')
                        {
                            line2 = line.substr((indexY-4) , indexY-3);
                            line3 = line.substr(indexY+1 , indexD-indexY-1);
                            line4 = line3.substr(0 , (line3.find('X')-1));
                            line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                            line5 = line3.substr(0,(line3.find('X')-1));
                            dcode_value.push_back((atof(line4.c_str()))/FOutputBmpMilPerPxl);
                            dcode_value.push_back((atof(line5.c_str()))/FOutputBmpMilPerPxl);

                            std::size_t found = line3.find('X');
                             if(found != std::string::npos )
                            {
                                 line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                                 line6 = line5 = line3.substr(0,(line3.find('X')-1));
                                 dcode_value.push_back((atof(line6.c_str())));
                            }
                             else
                             {
                                 line3.erase();

                             }

                            D_symbol_code.insert(std::pair<string , vector<float>>( line2,dcode_value ));
                            symbol.insert(std::pair<string ,char >( line.substr((indexY-4) , indexY-4),line2[3]));
                            dcode_value.clear();
                            line3.erase();
                        }
                        else if(line.at(indexY-1) == 'P')
                        {
                            line2 = line.substr((indexY-4) , indexY-3);
                            line3 = line.substr(indexY+1 , indexD-indexY-1);
                            line4 = line3.substr(0 , (line3.find('X')-1));
                            line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                            line5 = line3.substr(0,(line3.find('X')-1));
                            dcode_value.push_back((atof(line4.c_str()))/FOutputBmpMilPerPxl);
                            dcode_value.push_back((atof(line5.c_str()))/FOutputBmpMilPerPxl);

                            std::size_t found = line3.find('X');
                             if(found != std::string::npos )
                            {
                                 line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                                 line6 = line5 = line3.substr(0,(line3.find('X')-1));
                                 line3 = line3.substr((line3.find('X'))+1, (line3.find('X'))-line3.length());
                                 line7 = line3.substr(0,(line3.find('X')-1));
                                 dcode_value.push_back((atof(line6.c_str()))/FOutputBmpMilPerPxl);
                                 dcode_value.push_back((atof(line7.c_str()))/FOutputBmpMilPerPxl);
                            }
                             else
                             {
                                 line3.erase();
                             }

                            D_symbol_code.insert(std::pair<string , vector<float>>( line2,dcode_value ));
                            symbol.insert(std::pair<string ,char >( line.substr((indexY-4) , indexY-4),line2[3]));
                            dcode_value.clear();
                            line3.erase();
                        }
                    }

                    if(line =="%MOIN*%")
                    {
//                        frac=25.4;
                    }
                    else if(line =="%MOMM*%")
                    {
//                        frac =(1.0/1000.0)*25.4;
                    }
                    else if(line =="%LPD*%")
                    {
                         lpd=1;
                         lpc=0;
                    }
                    else if(line =="%LPC*%")
                    {
                        lpc=1;
                        lpd=0;
                    }
                }
// >>>>>>>>>>>>>>End of % section >>>>>>>>>>>>>>>>>>>>>>>>>//

//>>>>>>>>>>>>>start  G36 command  PCB white board>>>>>>>>>>>>>//
                if( (line == "G36*" || g_36 ==1 || line == "G37*"  ) && lpd  )
                {
                    if(line.compare("G36*") == 0)
                    {
                        if(g_36)
                        {
                             g_36=1;
                        }
                        if(g_54)
                        {
                             g_36=0;
                        }
                        g_36=1;
                        g_54=0;
                    }
                    else if(line.compare("G37*") == 0)
                    {
                        g_36=0;
                        g_54=1;
                    }

                    if( indexX >=0  &&  g_36  )
                    {
                        if(indexY>=0  && indexI<=0 && indexJ <=0)
                        {
                            Xorigin= line.substr(indexX+1,(indexY-indexX-1));
                            Yorigin = line.substr(indexY+1,(indexD-indexY-1));
                        }
                        else if(indexI>=0 && indexJ >=0)
                        {
                            Xorigin= line.substr(indexX+1,(indexY-indexX-1));
                            Yorigin = line.substr(indexY+1,(indexI-indexY-1));
                            Iorigin= line.substr(indexI+1,(indexJ-indexI-1));
                            Jorigin = line.substr(indexJ+1,(indexD-indexJ-1));
                        }
                        else
                        {
                            Xorigin= line.substr(indexX+1,(indexD-indexX-1));
                            y_core =cordinate[cordinate.size()-1].y;
                        }

                        if(Jorigin[0]== '-')
                        {
                            Jorigin = Jorigin.substr(1,Jorigin.length());
                            flag_j = 1;
                        }
                        if(Iorigin[0]== '-')
                        {
                              Iorigin = Iorigin.substr(1,Iorigin.length());
                              flag_i= 1;
                        }
                        if(Xorigin[0]== '-')
                        {
                              Xorigin = Xorigin.substr(1,Xorigin.length());
                              flag_x = 1;
                        }
                        if(Yorigin[0]== '-')
                        {
                              Yorigin = Yorigin.substr(1,Yorigin.length());
                              flag_y = 1;
                        }

                    // >>>>>>>>>>>>> Add zero if frc value small >>>>>>>>>>>>>>>>>>//

                        int lx=xf-Xorigin.length();
                        int ly=yf-Yorigin.length();
                        int li=xf-Iorigin.length();
                        int lj=yf-Jorigin.length();

                        // >>>>>>>>>>>>>>>>X value fac >>>>>>>>>>>>>>//
                        if(indexX>=0)
                        {
                            if(lx >= 0)
                            {
                                x_dec='0';
                                if(!(xf-Xorigin.length()))
                                {
                                    x_frac=Xorigin;
                                }
                                if(lx > 0)
                                {
                                    while(lx)
                                    {
                                        Xorigin =zero+Xorigin;
                                        lx--;
                                    }
                                    x_frac=Xorigin;
                                }
                            }
                            else
                            {
                                x_frac=Xorigin.substr(Xorigin.length()-xf,xf);
                                x_dec=Xorigin.substr(0,Xorigin.length()-xf);
                            }
                            x_point=x_dec+"."+x_frac;
                            x_core = stof(x_point);
                            x_core = x_core/FOutputBmpMilPerPxl;
                        }
                        //>>>>>>>>>>>>>>>>> End >>>>>>>>>>>>>>>>//

                        // >>>>>>>>>>>>>>>> I  value fac >>>>>>>>>>>>>//
                        if(indexI>=0)
                        {
                            if(li >= 0)
                            {
                                x_dec='0';
                                if(!(xf-Iorigin.length()))
                                {
                                    x_frac=Iorigin;
                                }
                                if(li > 0)
                                {
                                    while(li)
                                    {
                                        Iorigin =zero+Iorigin;
                                        li--;
                                    }
                                    x_frac=Iorigin;
                                }
                            }
                            else
                            {
                                x_frac=Iorigin.substr(Iorigin.length()-xf,xf);
                                x_dec=Iorigin.substr(0,Iorigin.length()-xf);
                            }
                            i_point=x_dec+"."+x_frac;
                            if (flag_i !=0 )
                                i_point='-'+i_point;
                            i_core = stof(i_point);
                            i_core = i_core/FOutputBmpMilPerPxl;
                        }
                        // >>>>>>>>>>>>>>>> End >>>>>>>>>>>>>>>>>>//

                        // >>>>>>>>>>>>>>>> J value fac >>>>>>>>>>>>>>//
                        if(indexJ>=0)
                        {
                            if(lj >= 0)
                            {
                                x_dec='0';
                                if(!(xf-Jorigin.length()))
                                {
                                    x_frac=Jorigin;
                                }
                                if(lj > 0)
                                {
                                    while(lj)
                                    {
                                        Jorigin =zero+Jorigin;
                                        lj--;
                                    }
                                    x_frac=Jorigin;
                                }
                            }
                            else
                            {
                                x_frac=Jorigin.substr(Jorigin.length()-yf,yf);
                                x_dec=Jorigin.substr(0,Jorigin.length()-yf);
                            }
                            j_point=x_dec+"."+x_frac;
                            if (flag_j !=0 )
                                j_point='-'+j_point;
                            j_core = stof(j_point);
                            j_core = j_core/FOutputBmpMilPerPxl;
                        }
                        // >>>>>>>>>>>>>>>> End >>>>>>>>>>>>>>>>>>>>>//

                        // >>>>>>>>>>>>>>>> Y value fac >>>>>>>>>>>>>>//
                        if(indexY>=0)
                        {
                            if(ly >= 0)
                            {
                                y_dec='0';
                                if(!(yf-Yorigin.length()))
                                {
                                    y_frac=Yorigin;
                                }
                                if(ly > 0)
                                {
                                    while(ly)
                                    {
                                        Yorigin =zero+Yorigin;
                                        ly--;
                                    }
                                    y_frac=Yorigin;
                                }
                            }
                            else
                            {
                                y_frac=Yorigin.substr(Yorigin.length()-yf,yf);
                                y_dec=Yorigin.substr(0,Yorigin.length()-yf);
                            }
                            y_point=y_dec+"."+y_frac;
                            y_core = stof(y_point);
                            y_core = y_core/FOutputBmpMilPerPxl;
                        }
                        //>>>>>>>>>>>>>>>>> End >>>>>>>>>>>>>>>>>>//
                        cv::Point temp_point;
                        temp_point.x = x_core;
                        temp_point.y = y_core;
                        cordinate.push_back(temp_point);

                        // >>>>>>>>>>> End  and Push into Cordinate only X,Y >>>>>>>>>>//
                        // >>>>>>>>>>>>>>End Fac >>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

                        //>>>>>>>>>>>>>> opencv draw part>>>>>>>>>>>>>>>>>>>>//

                        dataset_plot ob ;
                        if(line.at(indexD+2) == '2')
                        {
                            continue;
                        }
                       else  if(line.at(indexD+2) == '1' && line[0] != 'G' )
                        {
                            vertices.push_back(temp_point);
//                            cv::line( surface , (temp_point), (cordinate[cordinate.size()-2]),cv::Scalar(255),5,cv::FILLED);
                        }
                        //***>>>>>>>Start Draw clockwise and anti-clockwise G02 ,G03>>>>>>>>>***//

                        else if(line[0] == 'G'&& line[2] == '2' )
                        {
                            //>>>>>>>>>>>> Anti-clockwise  Arc>>>>>>>>>>>>>//
                            bool cw = true;

                            double x1 = temp_point.x;
                            double y1 = temp_point.y;

                            double x2 = (cordinate[cordinate.size()-2]).x;
                            double y2 = (cordinate[cordinate.size()-2]).y;

                            double c1 = x1 + i_core ;
                            double c2 = y1 + j_core;

                            vector<cv::Point> vertices;
                            ob.arc_new(surface , x1 , y1 , x2 ,y2 , c1 , c2 , cw , vertices);
                        }

                        else if  (line[0] == 'G' && line[2] == '3' )
                        {
                            //>>>>>>>>>>>>>>>> clockwise Arc>>>>>>>>>>>>>>>>//
                            bool cw = false;

                            double x1 = temp_point.x;
                            double y1 = temp_point.y;

                            double x2 = (cordinate[cordinate.size()-2]).x;
                            double y2 = (cordinate[cordinate.size()-2]).y;

                            double c1 = x1 + i_core ;
                            double c2 = y1 + j_core;

                            vector<cv::Point> vertices;
                            ob.arc_new(surface , x1 , y1 , x2 ,y2 , c1 , c2 , cw , vertices);
                        }
                        else if  (line[0] == 'G' && line[2] == '1' )
                        {
                            vertices.push_back(temp_point);
//                            cv::line( surface , (temp_point), (cordinate[cordinate.size()-2]),cv::Scalar(255),5,cv::FILLED);
                        }
                       //>>>>>>>>>>>>>>>>>>End clockwise nad anti-clockwise>>>>>>>>>>>>>>>>>>>>>>>>//

                    }
                    //>>>>>>>>>>>>>>>>>>>>> End Opencv Draw part >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>gerber G36 Command >>>>>>>>>>>>>>>>>>>>>>>>>>>>//
                    else if(( indexY >=0 )&&((g_36)||(g_01))   )
                    {
                        Yorigin= line.substr(indexY+1,(indexD-indexY-1));
                        x_core =cordinate[cordinate.size()-1].x;

                        int lx=xf-Xorigin.length();
                        int ly=yf-Yorigin.length();

                        if(ly >= 0)
                        {
                            y_dec='0';
                            if(!(yf-Yorigin.length()))
                            {
                                y_frac=Yorigin;
                            }
                            if(ly > 0)
                            {
                                while(ly)
                                {
                                    Yorigin =zero+Yorigin;
                                    ly--;
                                }
                                y_frac=Yorigin;
                            }
                        }
                        else
                        {
                            y_frac=Yorigin.substr(Yorigin.length()-yf,yf);
                            y_dec=Yorigin.substr(0,Yorigin.length()-yf);
                        }
                        y_point=y_dec+"."+y_frac;
                        y_core = stof(y_point);
                        y_core = y_core/FOutputBmpMilPerPxl;

                        cv::Point temp_point;
                        temp_point.x = x_core;
                        temp_point.y = y_core;
                        cordinate.push_back(temp_point);

                        if(line.at(indexD+2) == '2')
                        {
                            continue;
                        }

                        else  if(line.at(indexD+2) == '1')
                        {
                            vertices.push_back(temp_point);
//                            cv::line( surface , (temp_point), (cordinate[cordinate.size()-2]),cv::Scalar(255),5,cv::FILLED);
                        }
                    }
                }

// >>>>>>>>>>>> End of G36 command  PCB black board>>>>> >>>>>>>>>>>//
                if( (line == "G36*" || g_36 ==1 || line == "G37*"  ) && lpc )
                {

                    if(line.compare("G36*") == 0)
                    {
                        if(g_36)
                        {
                             g_36=1;
                        }
                        if(g_54)
                        {
                             g_36=0;
                        }
                        g_36=1;
                        g_54=0;
                    }
                    else if(line.compare("G37*") == 0)
                    {
                        g_36=0;
                        g_54=1;
                    }

                    if( indexX >=0  &&  g_36  )
                    {
                        if(indexY>=0  && indexI<=0 && indexJ <=0)
                        {
                            Xorigin= line.substr(indexX+1,(indexY-indexX-1));
                            Yorigin = line.substr(indexY+1,(indexD-indexY-1));
                        }
                        else if(indexI>=0 && indexJ >=0)
                        {
                            Xorigin= line.substr(indexX+1,(indexY-indexX-1));
                            Yorigin = line.substr(indexY+1,(indexI-indexY-1));
                            Iorigin= line.substr(indexI+1,(indexJ-indexI-1));
                            Jorigin = line.substr(indexJ+1,(indexD-indexJ-1));
                        }
                        else
                        {
                            Xorigin= line.substr(indexX+1,(indexD-indexX-1));
                            y_core =cordinate[cordinate.size()-1].y;
                        }

                        if(Jorigin[0]== '-')
                        {
                            Jorigin = Jorigin.substr(1,Jorigin.length());
                            flag_j = 1;
                        }
                        if(Iorigin[0]== '-')
                        {
                              Iorigin = Iorigin.substr(1,Iorigin.length());
                              flag_i= 1;
                        }
                        if(Xorigin[0]== '-')
                        {
                              Xorigin = Xorigin.substr(1,Xorigin.length());
                              flag_x = 1;
                        }
                        if(Yorigin[0]== '-')
                        {
                              Yorigin = Yorigin.substr(1,Yorigin.length());
                              flag_y = 1;
                        }

                    // >>>>>>>>>>>>> Add zero if frc value small >>>>>>>>>>>>>>>>>>//

                        int lx=xf-Xorigin.length();
                        int ly=yf-Yorigin.length();
                        int li=xf-Iorigin.length();
                        int lj=yf-Jorigin.length();

                        // >>>>>>>>>>>>>>>>X value fac >>>>>>>>>>>>>>//
                        if(indexX>=0)
                        {
                            if(lx >= 0)
                            {
                                x_dec='0';
                                if(!(xf-Xorigin.length()))
                                {
                                    x_frac=Xorigin;
                                }
                                if(lx > 0)
                                {
                                    while(lx)
                                    {
                                        Xorigin =zero+Xorigin;
                                        lx--;
                                    }
                                    x_frac=Xorigin;
                                }
                            }
                            else
                            {
                                x_frac=Xorigin.substr(Xorigin.length()-xf,xf);
                                x_dec=Xorigin.substr(0,Xorigin.length()-xf);
                            }
                            x_point=x_dec+"."+x_frac;
                            x_core = stof(x_point);
                            x_core = x_core/FOutputBmpMilPerPxl;
                        }
                        //>>>>>>>>>>>>>>>>> End >>>>>>>>>>>>>>>>//

                        // >>>>>>>>>>>>>>>> I  value fac >>>>>>>>>>>>>//
                        if(indexI>=0)
                        {
                            if(li >= 0)
                            {
                                x_dec='0';
                                if(!(xf-Iorigin.length()))
                                {
                                    x_frac=Iorigin;
                                }
                                if(li > 0)
                                {
                                    while(li)
                                    {
                                        Iorigin =zero+Iorigin;
                                        li--;
                                    }
                                    x_frac=Iorigin;
                                }
                            }
                            else
                            {
                                x_frac=Iorigin.substr(Iorigin.length()-xf,xf);
                                x_dec=Iorigin.substr(0,Iorigin.length()-xf);
                            }
                            i_point=x_dec+"."+x_frac;
                            if (flag_i !=0 )
                                i_point='-'+i_point;
                            i_core = stof(i_point);
                            i_core = i_core/FOutputBmpMilPerPxl;
                        }
                        // >>>>>>>>>>>>>>>> End >>>>>>>>>>>>>>>>>>//

                        // >>>>>>>>>>>>>>>> J value fac >>>>>>>>>>>>>>//
                        if(indexJ>=0)
                        {
                            if(lj >= 0)
                            {
                                x_dec='0';
                                if(!(xf-Jorigin.length()))
                                {
                                    x_frac=Jorigin;
                                }
                                if(lj > 0)
                                {
                                    while(lj)
                                    {
                                        Jorigin =zero+Jorigin;
                                        lj--;
                                    }
                                    x_frac=Jorigin;
                                }
                            }
                            else
                            {
                                x_frac=Jorigin.substr(Jorigin.length()-yf,yf);
                                x_dec=Jorigin.substr(0,Jorigin.length()-yf);
                            }
                            j_point=x_dec+"."+x_frac;
                            if (flag_j !=0 )
                                j_point='-'+j_point;
                            j_core = stof(j_point);
                            j_core = j_core/FOutputBmpMilPerPxl;
                        }
                        // >>>>>>>>>>>>>>>> End >>>>>>>>>>>>>>>>>>>>>//

                        // >>>>>>>>>>>>>>>> Y value fac >>>>>>>>>>>>>>//
                        if(indexY>=0)
                        {
                            if(ly >= 0)
                            {
                                y_dec='0';
                                if(!(yf-Yorigin.length()))
                                {
                                    y_frac=Yorigin;
                                }
                                if(ly > 0)
                                {
                                    while(ly)
                                    {
                                        Yorigin =zero+Yorigin;
                                        ly--;
                                    }
                                    y_frac=Yorigin;
                                }
                            }
                            else
                            {
                                y_frac=Yorigin.substr(Yorigin.length()-yf,yf);
                                y_dec=Yorigin.substr(0,Yorigin.length()-yf);
                            }
                            y_point=y_dec+"."+y_frac;
                            y_core = stof(y_point);
                            y_core = y_core/FOutputBmpMilPerPxl;
                        }
                        //>>>>>>>>>>>>>>>>> End >>>>>>>>>>>>>>>>>>//
                        cv::Point temp_point;
                        temp_point.x = x_core;
                        temp_point.y = y_core;
                        cordinate.push_back(temp_point);
                        vector<cv::Point> temp;
                        fill.push_back(temp);

                        // >>>>>>>>>>> End  and Push into Cordinate only X,Y >>>>>>>>>>//
                        // >>>>>>>>>>>>>>End Fac >>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

                        //>>>>>>>>>>>>>> opencv draw part>>>>>>>>>>>>>>>>>>>>//

                        dataset_plot ob ;
                        if(line.at(indexD+2) == '2')
                        {
                            continue;
                        }
                       else  if(line.at(indexD+2) == '1' && line[0] != 'G' )
                        {
                            cv::line( surface , (temp_point), (cordinate[cordinate.size()-2]),cv::Scalar(255),5, 8 );
                        }
                        //***>>>>>>>Start Draw clockwise and anti-clockwise G02 ,G03>>>>>>>>>***//

                        else if(line[0] == 'G'&& line[2] == '2' )
                        {
                            //>>>>>>>>>>>> Anti-clockwise  Arc>>>>>>>>>>>>>//
                            bool cw = true;

                            double x1 = temp_point.x;
                            double y1 = temp_point.y;

                            double x2 = (cordinate[cordinate.size()-2]).x;
                            double y2 = (cordinate[cordinate.size()-2]).y;

                            double c1 = x1 + i_core ;
                            double c2 = y1 + j_core;

                            vector<cv::Point> vertices;
                            ob.arc_new(surface , x1 , y1 , x2 ,y2 , c1 , c2 , cw , vertices);
                        }

                        else if  (line[0] == 'G' && line[2] == '3' )
                        {
                            //>>>>>>>>>>>>>>>> clockwise Arc>>>>>>>>>>>>>>>>//
                            bool cw = false;

                            double x1 = temp_point.x;
                            double y1 = temp_point.y;

                            double x2 = (cordinate[cordinate.size()-2]).x;
                            double y2 = (cordinate[cordinate.size()-2]).y;

                            double c1 = x1 + i_core ;
                            double c2 = y1 + j_core;

                            vector<cv::Point> vertices;
                            ob.arc_new(surface , x1 , y1 , x2 ,y2 , c1 , c2 , cw , vertices);
                        }
                        else if  (line[0] == 'G' && line[2] == '1' )
                        {
                            cv::line( surface , (temp_point), (cordinate[cordinate.size()-2]),cv::Scalar(255),5, 8);
                        }
                       //>>>>>>>>>>>>>>>>>>End clockwise nad anti-clockwise>>>>>>>>>>>>>>>>>>>>>>>>//

                    }
                    //>>>>>>>>>>>>>>>>>>>>> End Opencv Draw part >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>gerber G36 Command >>>>>>>>>>>>>>>>>>>>>>>>>>>>//
                    else if(( indexY >=0 )&&((g_36)||(g_01))   )
                    {
                        Yorigin= line.substr(indexY+1,(indexD-indexY-1));
                        x_core =cordinate[cordinate.size()-1].x;

                        int lx=xf-Xorigin.length();
                        int ly=yf-Yorigin.length();

                        if(ly >= 0)
                        {
                            y_dec='0';
                            if(!(yf-Yorigin.length()))
                            {
                                y_frac=Yorigin;
                            }
                            if(ly > 0)
                            {
                                while(ly)
                                {
                                    Yorigin =zero+Yorigin;
                                    ly--;
                                }
                                y_frac=Yorigin;
                            }
                        }
                        else
                        {
                            y_frac=Yorigin.substr(Yorigin.length()-yf,yf);
                            y_dec=Yorigin.substr(0,Yorigin.length()-yf);
                        }
                        y_point=y_dec+"."+y_frac;
                        y_core = stof(y_point);
                        y_core = y_core/FOutputBmpMilPerPxl;

                        cv::Point temp_point;
                        temp_point.x = x_core;
                        temp_point.y = y_core;
                        cordinate.push_back(temp_point);
                        vector<cv::Point> temp;
                        fill.push_back(temp);

                        if(line.at(indexD+2) == '2')
                        {
                            continue;
                        }

                        else  if(line.at(indexD+2) == '1')
                        {
                            cv::line( surface , (temp_point), (cordinate[cordinate.size()-2]),cv::Scalar(255),5, 8 );
                        }
                    }
                }


//************* start G54 with PCB white board***************************//
             if ((line.find("G54") == 0 || g_54) && lpd)
                {
                    if((g_54==1)&&(line.find("G54")!=0))
                    {
                        unshap_vertices.push_back(vertices);
                        cv::drawContours(surface  ,unshap_vertices  , 0 ,   cv::Scalar(255 ) ,CV_FILLED, 8 );
                        unshap_vertices.clear();
                        vertices.clear();
                        g_36 = 0;
                        g_54 = 1;
                    }
                    else if((g_54==1)&&(line.find("G54")==0))
                    {

                        g_36 = 0;
                        g_54 = 1;
                    }
                    else if(line.find("G54")==0)
                    {
                        if(g_36)
                        {

                        }
                        g_36 = 0;
                        g_54 = 1;
                    }
                    if(line.length()==7)
                    {
                         D_code_key = line.substr(3,line.length()-4) ;
                         D_code_key = D_code_key+symbol[D_code_key];
                    }
                    if( indexX >=0 )
                    {

                        if(indexY>=0)
                        {
                            Xorigin= line.substr(indexX+1,(indexY-indexX-1));
                            Yorigin = line.substr(indexY+1,(indexD-indexY-1));
                        }
                        else
                        {
                            Xorigin= line.substr(indexX+1,(indexD-indexX-1));
                            y_core =cordinate[cordinate.size()-1].y;

                        }
                        int lx=xf-Xorigin.length();
                        int ly=yf-Yorigin.length();

                        if(indexX>=0)
                        {
                            if(lx >= 0)
                            {
                                x_dec='0';
                                if(!(xf-Xorigin.length()))
                                {
                                    x_frac=Xorigin;
                                }
                                if(lx > 0)
                                {
                                    while(lx)
                                    {
                                        Xorigin =zero+Xorigin;
                                        lx--;
                                    }
                                    x_frac=Xorigin;
                                }
                            }
                            else
                            {
                                x_frac=Xorigin.substr(Xorigin.length()-xf,xf);
                                x_dec=Xorigin.substr(0,Xorigin.length()-xf);
                            }
                            x_point=x_dec+"."+x_frac;
                            x_core = stof(x_point)/FOutputBmpMilPerPxl;
                        }
                        if(indexY>=0)
                        {
                            if(ly >= 0)
                            {
                                y_dec='0';
                                if(!(yf-Yorigin.length()))
                                {
                                    y_frac=Yorigin;
                                }
                                if(ly > 0)
                                {
                                    while(ly)
                                    {
                                        Yorigin =zero+Yorigin;
                                        ly--;
                                    }
                                    y_frac=Yorigin;
                                }
                            }
                            else
                            {
                                y_frac=Yorigin.substr(Yorigin.length()-yf,yf);
                                y_dec=Yorigin.substr(0,Yorigin.length()-yf);
                            }
                            y_point=y_dec+"."+y_frac;
                            y_core = stof(y_point)/FOutputBmpMilPerPxl;
                        }
                        cv::Point temp_point;
                        temp_point.x = x_core;
                        temp_point.y = y_core;
                        cordinate.push_back(temp_point);

                        if(line.at(indexD+2) == '1')
                        {
                            if(D_code_key[3]=='R')
                            {
                                std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                                cv::rectangle(surface, cv::Rect(temp_point.x,temp_point.y,vec_vlaue[0],vec_vlaue[1]) , cv::Scalar(255) , 2 , CV_FILLED);
                            }

                            else if(D_code_key[3]=='C')
                            {
                                std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                                cv::line(surface , temp_point , (cordinate[cordinate.size()-2]) , cv::Scalar(255) ,vec_vlaue[0] ,cv::LINE_8);
                            }
                        }

                       else  if(line.at(indexD+2) == '2')
                        {
                            continue;
                        }
                        else if(line.at(indexD+2) == '3')
                        {

                            std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                            cv::circle(surface,   temp_point , vec_vlaue[0] ,  cv::Scalar(255,255,255), -1,CV_FILLED );

                       }

                    }

                    else if( indexY >=0 )
                    {

                        Yorigin= line.substr(indexY+1,(indexD-indexY-1));
                        x_core =cordinate[cordinate.size()-1].x;

                        int lx=xf-Xorigin.length();
                        int ly=yf-Yorigin.length();

                        if(ly >= 0)
                        {
                            y_dec='0';
                            if(!(yf-Yorigin.length()))
                            {
                                y_frac=Yorigin;
                            }
                            if(ly > 0)
                            {
                                while(ly)
                                {
                                    Yorigin =zero+Yorigin;
                                    ly--;
                                }
                                y_frac=Yorigin;
                            }
                        }
                        else
                        {
                            y_frac=Yorigin.substr(Yorigin.length()-yf,yf);
                            y_dec=Yorigin.substr(0,Yorigin.length()-yf);
                        }
                        y_point=y_dec+"."+y_frac;
                        y_core = stof(y_point)/FOutputBmpMilPerPxl;

                        cv::Point temp_point;
                        temp_point.x = x_core;
                        temp_point.y = y_core;
                        cordinate.push_back(temp_point);

                        if(line.at(indexD+2) == '1')
                        {
                            if(line.at(indexD+2) == '2')
                            {
                                continue;
                            }
                            else  if(D_code_key[3]=='R')
                            {
                                std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                                cv::rectangle(surface, cv::Rect(temp_point.x,temp_point.y,vec_vlaue[0],vec_vlaue[1]) , cv::Scalar(255) , 2 , CV_FILLED);
                            }
                            else if(D_code_key[3]== 'C' )
                            {
                                std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                                 cv::line(surface , temp_point , (cordinate[cordinate.size()-2]) , cv::Scalar(255) ,vec_vlaue[0] ,cv::LINE_8);
//                                vertices.push_back(temp_point);
//                                unshap_vertices.push_back(vertices);
                            }
                            else if(D_code_key[3]=='O')
                            {

                            }
                        }
                        else if(line.at(indexD+2) == '3')
                        {
                            std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                            cv::circle(surface,   temp_point , vec_vlaue[0] ,  cv::Scalar(255,255,255), -1, CV_FILLED);
                        }
                    }
                }
//>>>>>>>>>>> End G54 D-code Command >>>>>>>>>>> //

//>>>>>>>>>>> G54 with PCB Black board>>>>>>>>>>>//
            else if ((line.find("G54") == 0 || g_54) && lpc )
                {
                    if((g_54==1)&&(line.find("G54")!=0))
                    {
                        g_36 = 0;
                        g_54 = 1;
                    }
                    else if((g_54==1)&&(line.find("G54")==0))
                    {

                        g_36 = 0;
                        g_54 = 1;
                    }
                    else if(line.find("G54")==0)
                    {
                        if(g_36)
                        {

                        }
                        g_36 = 0;
                        g_54 = 1;
                    }
                    if(line.length()==7)
                    {
                         D_code_key = line.substr(3,line.length()-4) ;
                         D_code_key = D_code_key+symbol[D_code_key];
                    }
                    if( indexX >=0 )
                    {

                        if(indexY>=0)
                        {
                            Xorigin= line.substr(indexX+1,(indexY-indexX-1));
                            Yorigin = line.substr(indexY+1,(indexD-indexY-1));
                        }
                        else
                        {
                            Xorigin= line.substr(indexX+1,(indexD-indexX-1));
                            y_core =cordinate[cordinate.size()-1].y;

                        }
                        int lx=xf-Xorigin.length();
                        int ly=yf-Yorigin.length();

                        if(indexX>=0)
                        {
                            if(lx >= 0)
                            {
                                x_dec='0';
                                if(!(xf-Xorigin.length()))
                                {
                                    x_frac=Xorigin;
                                }
                                if(lx > 0)
                                {
                                    while(lx)
                                    {
                                        Xorigin =zero+Xorigin;
                                        lx--;
                                    }
                                    x_frac=Xorigin;
                                }
                            }
                            else
                            {
                                x_frac=Xorigin.substr(Xorigin.length()-xf,xf);
                                x_dec=Xorigin.substr(0,Xorigin.length()-xf);
                            }
                            x_point=x_dec+"."+x_frac;
                            x_core = stof(x_point)/FOutputBmpMilPerPxl;
                        }
                        if(indexY>=0)
                        {
                            if(ly >= 0)
                            {
                                y_dec='0';
                                if(!(yf-Yorigin.length()))
                                {
                                    y_frac=Yorigin;
                                }
                                if(ly > 0)
                                {
                                    while(ly)
                                    {
                                        Yorigin =zero+Yorigin;
                                        ly--;
                                    }
                                    y_frac=Yorigin;
                                }
                            }
                            else
                            {
                                y_frac=Yorigin.substr(Yorigin.length()-yf,yf);
                                y_dec=Yorigin.substr(0,Yorigin.length()-yf);
                            }
                            y_point=y_dec+"."+y_frac;
                            y_core = stof(y_point)/FOutputBmpMilPerPxl;
                        }
                        cv::Point temp_point;
                        temp_point.x = x_core;
                        temp_point.y = y_core;
                        cordinate.push_back(temp_point);

                        if(line.at(indexD+2) == '1')
                        {
                            if(D_code_key[3]=='R')
                            {
                                std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                                cv::rectangle(surface, cv::Rect(temp_point.x,temp_point.y,vec_vlaue[0],vec_vlaue[1]) , cv::Scalar(255) , -1 , cv::LINE_8 );
                            }

                            else if(D_code_key[3]=='C')
                            {
                                std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                                cv::line(surface , temp_point , (cordinate[cordinate.size()-2]) , cv::Scalar(255) ,vec_vlaue[0] ,cv::LINE_8);


                            }
                        }

                       else  if(line.at(indexD+2) == '2')
                        {
                            continue;
                        }
                        else if(line.at(indexD+2) == '3')
                        {

                            std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                            cv::circle(surface,   temp_point , vec_vlaue[0] ,  cv::Scalar(255), -1 );

                       }

                    }

                    else if( indexY >=0 )
                    {

                        Yorigin= line.substr(indexY+1,(indexD-indexY-1));
                        x_core =cordinate[cordinate.size()-1].x;

                        int lx=xf-Xorigin.length();
                        int ly=yf-Yorigin.length();

                        if(ly >= 0)
                        {
                            y_dec='0';
                            if(!(yf-Yorigin.length()))
                            {
                                y_frac=Yorigin;
                            }
                            if(ly > 0)
                            {
                                while(ly)
                                {
                                    Yorigin =zero+Yorigin;
                                    ly--;
                                }
                                y_frac=Yorigin;
                            }
                        }
                        else
                        {
                            y_frac=Yorigin.substr(Yorigin.length()-yf,yf);
                            y_dec=Yorigin.substr(0,Yorigin.length()-yf);
                        }
                        y_point=y_dec+"."+y_frac;
                        y_core = stof(y_point)/FOutputBmpMilPerPxl;

                        cv::Point temp_point;
                        temp_point.x = x_core;
                        temp_point.y = y_core;
                        cordinate.push_back(temp_point);

                        if(line.at(indexD+2) == '1')
                        {
                            if(line.at(indexD+2) == '2')
                            {
                                continue;
                            }
                            else  if(D_code_key[3]=='R')
                            {
                                std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                                cv::rectangle(surface, cv::Rect(temp_point.x,temp_point.y,vec_vlaue[0],vec_vlaue[1]) , cv::Scalar(255) , -1 , cv::LINE_8 , 0);
                            }
                            else if(D_code_key[3]== 'C' )
                            {
                                std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                                 cv::line(surface , temp_point , (cordinate[cordinate.size()-2]) , cv::Scalar(255) ,vec_vlaue[0] ,cv::LINE_8);

                            }
                            else if(D_code_key[3]=='O')
                            {

                            }
                        }
                        else if(line.at(indexD+2) == '3')
                        {
                            std::vector<float>  vec_vlaue  =  D_symbol_code[D_code_key];
                            cv::circle(surface,   temp_point , vec_vlaue[0] ,  cv::Scalar(255), -1);
                        }
                    }
                }

//>>>>>>>>>>>G54 Dark End>>>>>>>>>>>
            }
        }
}

//>>>>>>>>>>>>start system command -remove system generated file>>>>>>> //


ss="rm -rf " + file_path+file;
std::system(ss.c_str());

//>>>>>>>>>>>End system command -remove system generated file>>>>>>> //

}


