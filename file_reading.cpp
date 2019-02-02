#include "file_reading.h"
void file_reading::face(string file_path , float &xmax ,float &ymax, float &ymin , float &xmin , bool &polarity )
{
      std::string file="symbol.txt";
      std::string ss= "cd "+ file_path + " && ls >" + file;
      std::system(ss.c_str());
      std::ifstream file_layer(file_path+file);
      std::ifstream file_draw_layer(file_path+file);

      std::string line,file_line ,Xorigin , Yorigin ,Iorigin , Jorigin ;
      int indexX = -1, indexY= -1 , indexI= -1 , indexJ= -1 , indexD = -1 ;
      float y_core =0 ,x_core =0  ;
      std::vector<float> y_cor_list;
      std::vector<float> x_cor_list;
      string x_dec , y_dec, x_frac, y_frac, zero="0", x_point, y_point ;

      while(getline(file_layer,file_line))
      {
          float xd,xf,yd,yf;
//          if (file_line !="symbol.txt" && file_line == "gtl" )
          if (file_line!="symbol.txt" )
          {
              // end flag section //
              float frac=0.0;
              map<string,float> D_code;
              map<string,string>D_symbol_code;
              map<string,int> flag_map;
              std::ifstream file(file_path+file_line);
              while(std::getline(file, line))
              {

                  if (line == "%IPPOS*%")
                  {
                      polarity= true;
                  }
                  if (line == "%IPNEG*%")
                  {
                      polarity= false;
                  }


                  if(line.at(0) =='%' && (line.at(1) =='F'))
                  {

                      indexX= line.find('X');
                      indexY= line.find('Y');
                      Xorigin = line.substr(indexX+1,(indexY-indexX-1));
                      Yorigin = line.substr(indexY+1,(indexY-indexX-1));
                      int x_temp = atof(Xorigin.c_str()) ;
                      int  y_temp = (atof(Yorigin.c_str())) ;
                      xf = x_temp % 10;
                      xd = x_temp  / 10 ;
                      yf = y_temp % 10 ;
                      yd = y_temp  / 10 ;

                  }

                  else if ( line.find('X') == 0 || line.find('Y') == 0 || line.find('X') == 3 || line.find('Y') == 3 )
                  {
                      indexX= line.find('X');
                      indexY= line.find('Y');
                      indexI = line.find('I');
                      indexJ = line.find('J');
                      indexD = line.find('D');

                      if(line.at(indexX+1)=='-' || line.at(indexY+1)=='-')
                      {
//                          couting++;
                          x_cor_list.push_back(0);
                          y_cor_list.push_back(0);
                          continue;
                      }
                      Xorigin = line.substr(indexX+1,(indexY-indexX-1));
                      Yorigin = line.substr(indexY+1,(indexY-indexX-1));
                      Iorigin= line.substr(indexI+1,(indexJ-indexI-1));
                      Jorigin = line.substr(indexJ+1,(indexD-indexJ-1));

                      if( indexX >=0 && indexI ==-1 )
                      {
                          if(indexY>=0)
                          {
                              Xorigin= line.substr(indexX+1,(indexY-indexX-1));
                              Yorigin = line.substr(indexY+1,(indexD-indexY-1));
                          }
                          else
                          {
                              Xorigin= line.substr(indexX+1,(indexD-indexX-1));
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
                              x_core = stof(x_point);
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
                              y_core = stof(y_point);
                          }


                          QPointF temp_point;

                              x_cor_list.push_back(x_core);
                              y_cor_list.push_back(y_core);

                      }
                      else if( indexI >=0 )
                      {
                          if(indexY>=0)
                          {
                              Xorigin= line.substr(indexX+1,(indexY-indexX-1));
                              Yorigin = line.substr(indexY+1,(indexI-indexY-1));
                          }
                          else
                          {
                              Xorigin= line.substr(indexX+1,(indexI-indexX-1));
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
                              x_core = stof(x_point);
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
                              y_core = stof(y_point);
                          }
                          QPointF temp_point;

                              x_cor_list.push_back(x_core);
                              y_cor_list.push_back(y_core);

                      }
                      else if( indexY >=0 )
                     {
                         Yorigin= line.substr(indexY+1,(indexD-indexY-1));
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

                         QPointF temp_point;

                             x_cor_list.push_back(x_core);
                             y_cor_list.push_back(y_core);

                     }

                  }
              }
          }
      }

      xmax  = *(std::max_element(x_cor_list.begin(),x_cor_list.end()));
      xmin = *(std::min_element(x_cor_list.begin(),x_cor_list.end()));
      ymax = *(std::max_element(y_cor_list.begin(),y_cor_list.end()));
      ymin = *(std::min_element(y_cor_list.begin(),y_cor_list.end()));
       ss="rm -rf " + file_path+file;
       std::system(ss.c_str());
}
