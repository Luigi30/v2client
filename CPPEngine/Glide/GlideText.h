#pragma once

#include <glide.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>
//#include <boost/algorithm/string.hpp>

void tlConSet( float minX, float minY, 
               float maxX, float maxY,
               int columns, int rows,
               int color );
int  tlConOutput( const char *fmt, ... );
void tlConClear();
void tlConRender();

void tlSetScreen(float width, float height);

float tlScaleX( float coord );
float tlScaleY( float coord );