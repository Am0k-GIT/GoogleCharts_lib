// Set ssid and pass for your WiFi network and enjoy visualization of 
// Google Charts for your IoT devices. Tested on ESP microcontrollers.

#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <GoogleCharts.h>

const char ssid[] = "MyWiFi";
const char pass[] = "MyPassword";

const char HTML_head[] PROGMEM = R"(<!DOCTYPE html> 
        <meta http-equiv='Content-Type' content=text/html; charset='utf-8'>
        <title>GoogleCharts Library Example Page</title>
        <style>
                form {margin: 0 auto;width: 600px;padding: 1em;border: 1px solid #ccc;border-radius: 1em; background-color: rgb(247, 245, 240);}
                h1 {text-align: center;}
                h3 {text-align: left;}
                ul {list-style: none;padding: 0;margin: 0;}
                table {border: 1px; width: 600px;align-items: center;}
                td,th {text-align: center; height: 26;}
                label {display: inline-block;width: 120px;text-align: right;}
                textarea {vertical-align: top;height: 5em;}
        </style>
        </head>
        <body bgcolor='DimGrey'>
        <form>
        <table>
                <tr>
                <th><a href=/>Main</a></th>
                <th><a href=/linechart>Line Chart</a></th>
                <th><a href=/piechart>Pie Chart</a></th>
                <th><a href=/areachart>Area Chart</a></th>
                <th><a href=/steppedareachart>Stepped Area Chart</a></th>
                </tr>
        </table>
        </form>
        <br>
        <form>)";

const char HTML_end[] PROGMEM = R"(
        </form>
        </body>
</html>)";

const char HTML_main[] PROGMEM = R"(
        <h3> Library methods: </h3>
        <p>
        <mark>GoogleCharts newDataChart(uint16_t points, bool extraData);</mark> - creating an object of the class 
        (points - the number of points of reference in the stack (FIFO), extraData - using an additional data column (by default false)).
        </p>
        <p>
        <mark>GoogleCharts.push(uint64_t / String X, float A, float B);</mark> - add a row of data to the stack 
        (X - 1 column of data, A - 2 column of data, B - 3 column of data (optional with extraData = true)).
        </p>
        <p>
        <mark>GoogleCharts.setLegendIntervale (uint8_t intervale);</mark> - label interval for x-axis values.
        </p>
        <p>
        <mark>GoogleCharts.setMinValue(bool useMinValue, int minValueA,  int minValueB);</mark> - using minimum values ​​for plotting charts 
        (useMinValue turns on the function, minValueA is the minimum value for the first chart, minValueB is the minimum value for the second chart (if extraData = true)).
        </p>
        <p>
        <mark>GoogleCharts.setCurveFunction(bool curveFunction);</mark> - graph smoothing, works only with Line Charts.
        </p>
        <p>
        <mark>GoogleCharts.setDateFormat(bool useDateFormat, String dateFormat);</mark> - use time formatting with preset 
        (default "HH: mm"), can be used to format unix epoch time.
        </p>
        <p>
        <mark>GoogleCharts.setLeftStyle(String label, String title = "", String HTMLcolor = "");</mark> - setting the style of the left Y-axis.
        </p>
        <p>
        <mark>GoogleCharts.setRightStyle(String label, String title = "", String HTMLcolor = "");</mark> - setting the style of the right Y-axis.
        </p>
        <p>
        <mark>GoogleCharts.setHorizontalTitle(String title);</mark> - top caption for Google Charts.
        </p>
        <p>
        <mark>GoogleCharts.setHorizontalLabel(String label);</mark> - X chart column name.
        </p>
        <p>
        <mark>GoogleCharts.ready();</mark> - returns bool true if the stack has data to display, bool false if the stack is empty.
        </p>
        <p>
        <mark>GoogleCharts.getCharts(uint8_t type, uint16_t width, uint16_t height);</mark> 
        - returns a string to be embedded in an HTML form (type - visualization type (0 - Line Chart, 1 - Pie Chart, 2 - Area Chart, 
        3 - Stepped Area Chart), width - render width (pixels), height - render height (pixels)).
        </p>
        <h3> Library version: 1.0.0</h3>
        <p>
        <a href="https://github.com/Am0k-GIT/GoogleCharts_lib">GitHub page</a>
        </p>)";


WiFiClient espClient;
ESP8266WebServer HttpServer(80);

GoogleCharts newLineChart(20, true);
GoogleCharts newPieChart(4);
GoogleCharts newAreaChart(12, true);
GoogleCharts newSteppedAreaChart(4, true);

void handleRoot() 
{
        String str(FPSTR(HTML_head));
        str += FPSTR(HTML_main);
        str += FPSTR(HTML_end);
        HttpServer.send ( 200, "text/html", str );
}

void handleLineChart() 
{
        newLineChart.setCurveFunction(true);
        newLineChart.setDateFormat(true);
        newLineChart.setLegendIntervale(5);
        newLineChart.setLeftStyle("Temperature", "Temperature, C", "green");
        newLineChart.setRightStyle("Pressure", "Pressure, mmHg", "orange");
        newLineChart.setHorizontalLabel("Time");
        String str(FPSTR(HTML_head));
        if (newLineChart.ready())
        {
                str += newLineChart.getCharts(0, 600, 450);
        }
        str += FPSTR(HTML_end);
        HttpServer.send ( 200, "text/html", str );
}

void handlePieChart() 
{
        newPieChart.setLeftStyle("Dev. tools");
        String str(FPSTR(HTML_head));
        if (newPieChart.ready())
        {
                str += newPieChart.getCharts(1, 600, 450);
        }
        str += FPSTR(HTML_end);
        HttpServer.send ( 200, "text/html", str );
}

void handleAreaChart() 
{
        newAreaChart.setMinValue(true, 63, 94);
        newAreaChart.setDateFormat(true, "MMM dd");
        newAreaChart.setLeftStyle("Gold", "XAU", "gold");
        newAreaChart.setRightStyle("Palladium", "XPD", "blue");
        newAreaChart.setHorizontalLabel("Time");
        newAreaChart.setLegendIntervale(3);
        String str(FPSTR(HTML_head));
        if (newAreaChart.ready())
        {
                str += newAreaChart.getCharts(2, 600, 450);
        }
        str += FPSTR(HTML_end);
        HttpServer.send ( 200, "text/html", str );
}

void handleSteppedAreaChart() 
{
        newSteppedAreaChart.setMinValue(true, 0, 0);
        newSteppedAreaChart.setLegendIntervale(1);
        newSteppedAreaChart.setHorizontalTitle(R"(The decline of \'The 39 Steps\')");
        newSteppedAreaChart.setHorizontalLabel("Director (Year)");
        newSteppedAreaChart.setLeftStyle("IDBM", "IDBM", "green");
        newSteppedAreaChart.setRightStyle("Rotten tomatoes", "Rotten tomatoes", "blue");
        String str(FPSTR(HTML_head));
        if (newSteppedAreaChart.ready())
        {
                str += newSteppedAreaChart.getCharts(3, 600, 450);
        }
        str += FPSTR(HTML_end);
        HttpServer.send ( 200, "text/html", str );
}

void handleNotFound() 
{
        HttpServer.send(404, "text/plain", "404: Not found");
}

void setup_WiFi()
{
         WiFi.mode( WIFI_STA);
        delay(10);
        WiFi.begin(ssid, pass);
        uint8_t attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 50) {
                attempts++;
                delay(200);
        }
}

void setup_HTTPserver() 
{
        HttpServer.on("/", handleRoot);
        HttpServer.on("/linechart", handleLineChart);
        HttpServer.on("/piechart", handlePieChart);
        HttpServer.on("/areachart", handleAreaChart);
        HttpServer.on("/steppedareachart", handleSteppedAreaChart);
        HttpServer.onNotFound(handleNotFound);
        HttpServer.begin();
}

void setup() 
{
        setup_WiFi();
        setup_HTTPserver();
        newLineChart.push(1627795528000, 16.87, 755.4);
        newLineChart.push(1627795628000, 16.91, 755.4);
        newLineChart.push(1627795728000, 16.95, 755.4);
        newLineChart.push(1627795828000, 16.92, 755.3);
        newLineChart.push(1627795928000, 16.87, 755.3);
        newLineChart.push(1627796028000, 16.67, 755.3);
        newLineChart.push(1627796128000, 16.32, 755.3);
        newLineChart.push(1627796228000, 16.12, 755.3);
        newLineChart.push(1627796328000, 16.01, 755.2);
        newLineChart.push(1627796428000, 15.87, 755.2);
        newLineChart.push(1627796528000, 15.67, 755.2);
        newLineChart.push(1627796628000, 15.73, 755.2);
        newLineChart.push(1627796728000, 16.02, 755.1);
        newLineChart.push(1627796828000, 16.17, 755.1);
        newLineChart.push(1627796928000, 16.23, 755.1);
        newLineChart.push(1627797028000, 16.25, 755.1);
        newLineChart.push(1627797128000, 16.30, 755.1);
        newLineChart.push(1627797228000, 16.32, 755.2);
        newLineChart.push(1627797328000, 16.35, 755.2);
        newLineChart.push(1627797428000, 16.37, 755.3);
        newLineChart.push(1627797528000, 16.40, 755.3);
        newLineChart.push(1627797628000, 16.42, 755.4);
        newLineChart.push(1627797728000, 16.41, 755.5);
        newLineChart.push(1627797828000, 16.42, 755.6);
        newPieChart.push("Java", 37);
        newPieChart.push("С/С++", 30);
        newPieChart.push("C#", 28);
        newPieChart.push("Python", 12);
        newAreaChart.push(1628025528000, 63.96, 94.18);
        newAreaChart.push(1628125528000, 63.97, 94.17);
        newAreaChart.push(1628225528000, 63.98, 94.16);
        newAreaChart.push(1628325528000, 63.97, 94.19);
        newAreaChart.push(1628425528000, 63.96, 94.22);
        newAreaChart.push(1628525528000, 63.95, 94.24);
        newAreaChart.push(1628625528000, 63.93, 94.28);
        newAreaChart.push(1628725528000, 63.91, 94.17);
        newAreaChart.push(1628825528000, 63.96, 94.18);
        newAreaChart.push(1628925528000, 63.97, 94.17);
        newAreaChart.push(1629025528000, 63.98, 94.16);
        newAreaChart.push(1629125528000, 63.97, 94.19);
        newSteppedAreaChart.push("Alfred Hitchcock 1935", 7.9, 8.4);
        newSteppedAreaChart.push("Ralph Thomas 1959", 6.5, 6.9);
        newSteppedAreaChart.push("Don Sharp 1978", 6.4, 6.5);
        newSteppedAreaChart.push("James Hawes 2008", 6.2, 4.4);
}

void loop() 
{
        HttpServer.handleClient();
}