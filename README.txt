Instructions
============

1. Prerequisites
   =============
   
- python
- octave
- R and Rmetrics. See www.rmetrics.org for instructions on how to install Rmetrics

2. Set-up
   ======
   
In Euclide/utils/octave/xmltoolbox
$ make

To build the Euclide framework
$ ant xml_to_java
$ ant

3. Test
   ====
   
$ java -jar ./build/Euclide-full.jar &

Open a workflow:

Workflow -> Open -> DeltaHedgeComparator

Run the calculation

Execution -> Run

All widgets should turn green, and you should get the message "The workflow has successfully been executed".
To inspect the results:
- select the icon next to the output "wealthPlot" in the HedgeComparator widget.
- in the lower panel, select "System's default file viewer".

4. Widget development
   ==================
   
- Widget definitions (xml files) are in Euclide/configuration/descriptions

- to rebuild Euclide, first regenerate the java source code for the widgets with
$ ant xml_to_java

- then rebuild the application with
$ ant

The environment variables used during widget execution are defined in configuration/platform/platform_shell

