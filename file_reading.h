#ifndef FILE_READING_H
#define FILE_READING_H

#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <limits>
#include <string>
#include <string>
#include <iostream>
#include "QGraphicsItem"
#include <QtSvg/QSvgRenderer>
#include <QtSvg/QGraphicsSvgItem>


using namespace std;




class file_reading
{
public:
       void face(string file_path , float &xmax ,float &ymax, float &ymin , float &xmin  , bool &polarity);

};

#endif // FILE_READING_H
