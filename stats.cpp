
#include "stats.h"

stats::stats(PNG& im) {
    /* Your code here!! */
    for (unsigned int x = 0; x < im.width(); x++) {
        vector<long> redtemp;
        vector<long> greentemp;
        vector<long> bluetemp;
        vector<long> redsqtemp;
        vector<long> greensqtemp;
        vector<long> bluesqtemp;

        for (unsigned int y = 0; y < im.height(); y++) {
            RGBAPixel *pixel = im.getPixel(x, y);
            int r = pixel->r;
            int g = pixel->g;
            int b = pixel->b;
            long rsq = pow((pixel->r), 2);
            long gsq = pow((pixel->g), 2);
            long bsq = pow((pixel->b), 2);

            int left = x-1;
            int top = y-1;

            if (left >= 0 && top >= 0) {
                r += sumRed[left][y] + redtemp[top] - sumRed[left][top];
                g += sumGreen[left][y] + greentemp[top] - sumGreen[left][top];
                b += sumBlue[left][y] + bluetemp[top] - sumBlue[left][top];
                rsq += sumsqRed[left][y] + redsqtemp[top] - sumsqRed[left][top]; 
                gsq += sumsqGreen[left][y] + greensqtemp[top] - sumsqGreen[left][top];
                bsq += sumsqBlue[left][y] + bluesqtemp[top] - sumsqBlue[left][top];
            } else if (left>=0) {
                r += sumRed[left][y];
                g += sumGreen[left][y];
                b += sumBlue[left][y];
                rsq += sumsqRed[left][y];
                gsq += sumsqGreen[left][y];
                bsq += sumsqBlue[left][y];
            } else if (top>=0) {
                r += redtemp[top];
                g += greentemp[top];
                b += bluetemp[top];
                rsq += redsqtemp[top];
                gsq += greensqtemp[top];
                bsq += bluesqtemp[top];
            } else {
                // nothing
            }
            redtemp.push_back(r);
            greentemp.push_back(g);
            bluetemp.push_back(b);
            redsqtemp.push_back(rsq);
            greensqtemp.push_back(gsq);
            bluesqtemp.push_back(bsq);
        }
        sumRed.push_back(redtemp);
        sumGreen.push_back(greentemp);
        sumBlue.push_back(bluetemp);
        sumsqRed.push_back(redsqtemp);
        sumsqGreen.push_back(greensqtemp);
        sumsqBlue.push_back(bluesqtemp);
    }
}

long stats::getSum(char channel, pair<int, int> ul, int dim) 
{
    /* Your code here!! */
    long sum = 0;
    int x = ul.first;
    int y = ul.second;
    int bot_x = x + pow(2, dim) - 1; // index of most right of rectangle
    int bot_y = y + pow(2, dim) - 1; // index of bottom of rectangle

    if (channel == 'r') {
        if (x == 0 && y == 0) {
            sum = sumRed[bot_x][bot_y];
        } else if (x == 0) {
            sum = sumRed[bot_x][bot_y] - sumRed[bot_x][y-1];
        } else if (y == 0) {
            sum = sumRed[bot_x][bot_y] - sumRed[x-1][bot_y];
        } else {
            sum = sumRed[bot_x][bot_y] - sumRed[x-1][bot_y] - sumRed[bot_x][y-1] + sumRed[x-1][y-1];
        }
    } else if (channel == 'g') {
        if (x == 0 && y == 0) {
            sum = sumGreen[bot_x][bot_y];
        } else if (x == 0) {
            sum = sumGreen[bot_x][bot_y] - sumGreen[bot_x][y-1];
        } else if (y == 0) {
            sum = sumGreen[bot_x][bot_y] - sumGreen[x-1][bot_y];
        } else {
            sum = sumGreen[bot_x][bot_y] - sumGreen[x-1][bot_y] - sumGreen[bot_x][y-1] + sumGreen[x-1][y-1];
        }
    } else {
        if (x == 0 && y == 0) {
            sum = sumBlue[bot_x][bot_y];
        } else if (x == 0) {
            sum = sumBlue[bot_x][bot_y] - sumBlue[bot_x][y-1];
        } else if (y == 0) {
            sum = sumBlue[bot_x][bot_y] - sumBlue[x-1][bot_y];
        } else {
            sum = sumBlue[bot_x][bot_y] - sumBlue[x-1][bot_y] - sumBlue[bot_x][y-1] + sumBlue[x-1][y-1];
        }
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {
    /* Your code here!! */
    long sumsq = 0;
    int x = ul.first;
    int y = ul.second;
    int bot_x = x + pow(2, dim) - 1; // index of most right of rectangle
    int bot_y = y + pow(2, dim) - 1; // index of bottom of rectangle

    if (channel == 'r') {
        if (x == 0 && y == 0) {
            sumsq = sumsqRed[bot_x][bot_y];
        } else if (x == 0) {
            sumsq = sumsqRed[bot_x][bot_y] - sumsqRed[bot_x][y-1];
        } else if (y == 0) {
            sumsq = sumsqRed[bot_x][bot_y] - sumsqRed[x-1][bot_y];
        } else {
            sumsq = sumsqRed[bot_x][bot_y] - sumsqRed[x-1][bot_y] - sumsqRed[bot_x][y-1] + sumsqRed[x-1][y-1];
        }
    } else if (channel == 'g') {
        if (x == 0 && y == 0) {
            sumsq = sumsqGreen[bot_x][bot_y];
        } else if (x == 0) {
            sumsq = sumsqGreen[bot_x][bot_y] - sumsqGreen[bot_x][y-1];
        } else if (y == 0) {
            sumsq = sumsqGreen[bot_x][bot_y] - sumsqGreen[x-1][bot_y];
        } else {
            sumsq = sumsqGreen[bot_x][bot_y] - sumsqGreen[x-1][bot_y] - sumsqGreen[bot_x][y-1] + sumsqGreen[x-1][y-1];
        }
    } else {
        if (x == 0 && y == 0) {
            sumsq = sumsqBlue[bot_x][bot_y];
        } else if (x == 0) {
            sumsq = sumsqBlue[bot_x][bot_y] - sumsqBlue[bot_x][y-1];
        } else if (y == 0) {
            sumsq = sumsqBlue[bot_x][bot_y] - sumsqBlue[x-1][bot_y];
        } else {
            sumsq = sumsqBlue[bot_x][bot_y] - sumsqBlue[x-1][bot_y] - sumsqBlue[bot_x][y-1] + sumsqBlue[x-1][y-1];
        }
    }
    return sumsq;
}

long stats::rectArea(int dim) {
    /* Your code here!! */
    int area = pow(2, dim) * pow(2, dim);
    return area;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {
    /* Your code here!! */
    double varRed = getSumSq('r', ul, dim)  - (pow(getSum('r', ul, dim), 2)/ rectArea(dim));
    double varGreen = getSumSq('g', ul, dim)  - (pow(getSum('g', ul, dim), 2)/ rectArea(dim));
    double varBlue = getSumSq('b', ul, dim) - (pow(getSum('b', ul, dim), 2)/ rectArea(dim));
    return varRed + varGreen + varBlue;
}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    /* Your code here!! */
    int avgRed = getSum('r', ul, dim)/(rectArea(dim));
    int avgGreen = getSum('g', ul, dim)/(rectArea(dim));
    int avgBlue = getSum('b', ul, dim)/(rectArea(dim));

    return RGBAPixel(avgRed, avgGreen, avgBlue);
}
