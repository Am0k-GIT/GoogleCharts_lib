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

#include "GoogleCharts.h"
#include <Arduino.h>

const char HTML_GoogleChartsAffix_p1[] PROGMEM = R"(
        <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
        <script type="text/javascript">
        google.charts.load('current', {'packages':['corechart']});
        google.charts.setOnLoadCallback(drawChart);
        function drawChart() {)";

const char HTML_GoogleChartsAffix_dateFormat[] PROGMEM = R"(
        var dateFormatter = new google.visualization.DateFormat({pattern: ')";

const char HTML_GoogleChartsAffix_dateFormatter[] PROGMEM = R"(dateFormatter.formatValue(new Date()";

const char HTML_GoogleChartsAffix_p2[] PROGMEM = R"(
        var data = google.visualization.arrayToDataTable([)";

const char HTML_GoogleChartsSuffix_p0[] PROGMEM = R"(])
        var options = {)";

const char HTML_GoogleChartsSuffix_curve[] PROGMEM = R"(
        curveType: 'function',)";

const char HTML_GoogleChartsSuffix_legend[] PROGMEM = R"(
        legend: { position: 'bottom' },)";

const char HTML_GoogleChartsSuffix_p1[] PROGMEM = R"(
        hAxis: {showTextEvery: )";

const char HTML_GoogleChartsSuffix_p2[] PROGMEM = R"(},
        vAxes: {)";
            
const char HTML_GoogleChartsSuffix_p3[] PROGMEM = R"( gridlines: {color: 'transparent'}})";

const char HTML_GoogleChartsSuffix_p4[] PROGMEM = R"(
        series: {
        0: {targetAxisIndex:0})";

const char HTML_GoogleChartsSuffix_p5[] PROGMEM = R"(
        1: {targetAxisIndex:1})";

const char HTML_GoogleChartsSuffix_p6[] PROGMEM = R"(
        var chart = new google.visualization.)";

const char HTML_GoogleChartsSuffix_p7[] = R"((document.getElementById('chart'));
        chart.draw(data, options);
        }
        </script>)";

GoogleCharts::GoogleCharts(uint16_t points, bool extraData)
{
    m_points = points;
    m_axisXvalue = new String[points];
    m_axisYvalueA = new float[points];
    m_extraData = extraData;
    if (extraData)
        m_axisYvalueB = new float[points];
    m_dateFormat = false;
    m_curveFunction = false;
    m_useMinValue = false;
    m_currentPoint = 0;
    m_ready = false;
    m_useString = false;
    m_intervale = 10;
}

GoogleCharts::~GoogleCharts()
{
    delete[] m_axisXvalue;
    delete[] m_axisYvalueA;
    delete[] m_axisYvalueB;
}

void GoogleCharts::set(uint16_t position, String X, float A)
{
    m_axisXvalue[position] = X;
    m_axisYvalueA[position] = A;
}

void GoogleCharts::set(uint16_t position, String X, float A, float B)
{
    m_axisXvalue[position] = X;
    m_axisYvalueA[position] = A;
    m_axisYvalueB[position] = B;
}

String GoogleCharts::getTable()
{
    String buffer = "";
    for (uint16_t i = 0; i < m_currentPoint; i++)
    {
        buffer += "\t\t[";
        if (m_useString)
        {
            buffer += R"(")";
            buffer += m_axisXvalue[i];
            buffer += R"(")";
        }
        else
        {
            if (m_dateFormat)
                buffer += FPSTR(HTML_GoogleChartsAffix_dateFormatter);
            buffer += m_axisXvalue[i];
            if (m_dateFormat)
                buffer += "))";
        }
        buffer += ", ";
        buffer += String(m_axisYvalueA[i]);
        if (m_extraData)
        {
            buffer += ", ";
            buffer += String(m_axisYvalueB[i]);
        }
        buffer += "],";
        buffer += "\n";
    }
    buffer += "\t\t";
    return (buffer);
}

void GoogleCharts::push(String X, float A)
{
    m_ready = true;
    m_useString = true;
    if (m_currentPoint < m_points)
    {
        set(m_currentPoint, X, A);
        m_currentPoint++;
    }
    else
    {
        for (uint16_t i = 0; i < m_points - 1; i++)
        {
            m_axisXvalue[i] = m_axisXvalue[i + 1];
            m_axisYvalueA[i] = m_axisYvalueA[i + 1];
        }
        set(m_currentPoint - 1, X, A);
    }
}

void GoogleCharts::push(String X, float A, float B)
{
    m_ready = true;
    m_useString = true;
    if (m_currentPoint < m_points)
    {
        set(m_currentPoint, X, A, B);
        m_currentPoint++;
    }
    else
    {
        for (uint16_t i = 0; i < m_points - 1; i++)
        {
            m_axisXvalue[i] = m_axisXvalue[i + 1];
            m_axisYvalueA[i] = m_axisYvalueA[i + 1];
            m_axisYvalueB[i] = m_axisYvalueB[i + 1];
        }
        set(m_currentPoint - 1, X, A, B);
    }
}

void GoogleCharts::push(uint64_t X, float A)
{
    m_ready = true;
    m_useString = false;
    if (m_currentPoint < m_points)
    {
        set(m_currentPoint, String(X), A);
        m_currentPoint++;
    }
    else
    {
        for (uint16_t i = 0; i < m_points - 1; i++)
        {
            m_axisXvalue[i] = m_axisXvalue[i + 1];
            m_axisYvalueA[i] = m_axisYvalueA[i + 1];
        }
        set(m_currentPoint - 1, String(X), A);
    }
}

void GoogleCharts::push(uint64_t X, float A, float B)
{
    m_ready = true;
    m_useString = false;
    if (m_currentPoint < m_points)
    {
        set(m_currentPoint, String(X), A, B);
        m_currentPoint++;
    }
    else
    {
        for (uint16_t i = 0; i < m_points - 1; i++)
        {
            m_axisXvalue[i] = m_axisXvalue[i + 1];
            m_axisYvalueA[i] = m_axisYvalueA[i + 1];
            m_axisYvalueB[i] = m_axisYvalueB[i + 1];
        }
        set(m_currentPoint - 1, String(X), A, B);
    }
}

void GoogleCharts::setLegendIntervale(uint8_t intervale)
{
    m_intervale = intervale;
}

void GoogleCharts::setCurveFunction(bool curveFunction)
{
    m_curveFunction = curveFunction;
}

void GoogleCharts::setDateFormat(bool useDateFormat, String dateFormat)
{
    m_useDateFormat = useDateFormat;
    m_dateFormat = dateFormat;
}

void  GoogleCharts::setMinValue(bool useMinValue, int minValueA)
{
    m_useMinValue = useMinValue;
    m_minValueA = minValueA;
}


void  GoogleCharts::setMinValue(bool useMinValue, int minValueA, int minValueB)
{
    m_useMinValue = useMinValue;
    m_minValueA = minValueA;
    m_minValueB = minValueB;
}

void GoogleCharts::setLeftStyle(String label, String title, String HTMLcolor)
{
    m_labelLeft = label;
    m_vtitleLeft = title;
    m_colorLeft = HTMLcolor;
}

void GoogleCharts::setRightStyle(String label, String title, String HTMLcolor)
{
    m_labelRight = label;
    m_vtitleRight = title;
    m_colorRight = HTMLcolor;
}

void GoogleCharts::setHorizontalTitle(String title)
{
    m_titleHor = title;
}

void GoogleCharts::setHorizontalLabel(String label)
{
    m_labelHor = label;
}

String GoogleCharts::getCharts(uint8_t type, uint16_t width, uint16_t height)
{
    switch (type)
    {
    case 0: 
    {
        m_type = LineChart;
        break;
    }
    case 1: 
    {
        m_type = PieChart;
        m_curveFunction = false;
        m_useMinValue = false;
        break;
    }
    case 2: 
    {
        m_type = AreaChart;
        m_curveFunction = false;
        break;
    }
    case 3: 
    {
        m_type = SteppedAreaChart;
        m_curveFunction = false;
        break;
    }
    default: 
    {
        m_type = LineChart;
        break;
    }
    }
    String buffer = "";
    buffer += FPSTR(HTML_GoogleChartsAffix_p1);
    if (m_useDateFormat)
    {
        buffer += FPSTR(HTML_GoogleChartsAffix_dateFormat);
        buffer += m_dateFormat;
        buffer += R"('});)";
    }
    buffer += FPSTR(HTML_GoogleChartsAffix_p2);
    buffer += "\n\t\t";
    buffer += "['";
    buffer += m_labelHor;
    buffer += "', '";
    buffer += m_labelLeft;
    if (m_extraData)
    {
        buffer += "', '";
        buffer += m_labelRight;
    }
    buffer += "'],";
    buffer += "\n";
    buffer += GoogleCharts::getTable();
    buffer += FPSTR(HTML_GoogleChartsSuffix_p0);
    if (m_titleHor.length() != 0)
    {
        buffer += "\n\t\t";
        buffer += R"(title: ')";
        buffer += m_titleHor;
        buffer += R"(',)";
    }
    buffer += FPSTR(HTML_GoogleChartsSuffix_legend);
    if (m_type != 1)
    {
        if (m_curveFunction)
            buffer += FPSTR(HTML_GoogleChartsSuffix_curve);   
        buffer += FPSTR(HTML_GoogleChartsSuffix_p1);
        buffer += String(m_intervale);
        buffer += FPSTR(HTML_GoogleChartsSuffix_p2);
        buffer += "\n\t\t";
        buffer += R"(0:{)";
        if (m_vtitleLeft.length() != 0)
        {
            buffer += R"(title: ')";
            buffer += m_vtitleLeft;
            buffer += R"(',)";
        }
        if (m_useMinValue)
        {
            buffer += R"( minValue: )"; 
            buffer += String(m_minValueA);
            buffer += R"(,)"; 
        }
        buffer += FPSTR(HTML_GoogleChartsSuffix_p3);
        if (m_extraData) {
            buffer += R"(,)";
            buffer += "\n\t\t";
            buffer += R"(1:{)";
            if (m_vtitleRight.length() != 0)
            {
                buffer += R"(title: ')";
                buffer += m_vtitleRight;
                buffer += R"(',)";
            }
            if (m_useMinValue)
            {
                buffer += R"( minValue: )"; 
                buffer += String(m_minValueB);
                buffer += R"(,)"; 
            }
            buffer += FPSTR(HTML_GoogleChartsSuffix_p3);
        }
        buffer += "\n\t\t";
        buffer += R"(},)"; 
        buffer += FPSTR(HTML_GoogleChartsSuffix_p4);
        if (m_extraData)
        {
            buffer += R"(,)";
            buffer += FPSTR(HTML_GoogleChartsSuffix_p5);
        }
        buffer += "\n\t\t";
        buffer += R"(},)";
        buffer += "\n\t\t";
        buffer += R"(colors: [")";
        buffer += m_colorLeft;
        if (m_extraData)
        {
            buffer += R"(", ")";
            buffer += m_colorRight;
        }
        buffer += R"("])";
    }
    buffer += "\n\t\t";
    buffer += R"(})";
    buffer += FPSTR(HTML_GoogleChartsSuffix_p6);
    switch (m_type)
    {
    case 0: buffer += "LineChart";
        break;
    case 1: buffer += "PieChart";
        break;
    case 2: buffer += "AreaChart";
        break;
    case 3: buffer += "SteppedAreaChart";
        break;
    default: buffer += "LineChart";
        break;
    }
    buffer += FPSTR(HTML_GoogleChartsSuffix_p7);
    buffer += R"(
        <div id="chart" style="width: )";
    buffer += String(width);
    buffer += R"(px; height: )";
    buffer += String(height);
    buffer += R"(px"></div>)";
    return (buffer);
}

bool GoogleCharts::ready()
{
    return (m_ready);
}