/*
 Google Chart library.
 Pavel "Am0k" Kolesov
 https://github.com/Am0k-GIT/GoogleCharts_lib

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU
 General Public License as published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version. This program is distributed in the hope that it
 will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. You should
 have received a copy of the GNU General Public License along with this program.  If not, see
 <http://www.gnu.org/licenses/>.
*/


#ifndef GoogleCharts_h
#define GoogleCharts_h

#include <Arduino.h>

class GoogleCharts
{
private:

    String* m_axisXvalue;
    float* m_axisYvalueA;
    float* m_axisYvalueB;
    bool m_extraData;
    bool m_useDateFormat;
    String m_dateFormat;
    bool m_useMinValue;
    int m_minValueA;
    int m_minValueB;
    bool m_curveFunction;
    bool m_useString;
    uint8_t m_intervale;
    uint16_t m_points;
    uint16_t m_currentPoint;
    bool m_ready;

    String m_vtitleLeft;
    String m_vtitleRight;
    String m_titleHor;
    String m_labelLeft;
    String m_labelRight;
    String m_labelHor;
    String m_colorLeft = "red";
    String m_colorRight = "blue";

    enum chartType
    {
        LineChart,
        PieChart,
        AreaChart,
        SteppedAreaChart
    };
    chartType m_type;

    void set(uint16_t position, String X, float A);
    void set(uint16_t position, String X, float A, float B);
    String getTable();

public:

    GoogleCharts(uint16_t points, bool extraData = false);
    ~GoogleCharts();
    void push(String X, float A);
    void push(String X, float A, float B);
    void push(uint64_t X, float A);
    void push(uint64_t X, float A, float B);
    void setLegendIntervale(uint8_t intervale);
    void setCurveFunction(bool curveFunction);
    void setDateFormat(bool useDateFormat, String dateFormat = "HH:mm");
    void setMinValue(bool useMinValue, int minValueA);
    void setMinValue(bool useMinValue, int minValueA,  int minValueB);
    /*
    void setVerticalTitle(String vtitle);
    void setVerticalTitle(String vtitleA, String vtitleB);
    */
    void setLeftStyle(String label, String title = "", String HTMLcolor = "");
    void setRightStyle(String label, String title = "", String HTMLcolor = "");
    void setHorizontalTitle(String title);
    void setHorizontalLabel(String label);
    String getCharts(uint8_t type, uint16_t width, uint16_t height);
    bool ready();

};

#endif