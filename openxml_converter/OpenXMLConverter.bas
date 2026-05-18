'' OpenXMLConverter.bas - Main program for converting OpenXML flowcharts to FreeBasic
'' This program reads OOXML drawing canvas files and generates executable FreeBasic code

#include "XMLParser.bas"
#include "FlowchartParser.bas"
#include "CodeGenerator.bas"

Function ShowUsage()
    Print "OpenXML Flowchart to FreeBasic Converter"
    Print "Usage: OpenXMLConverter <input.xml> [output.bas]"
    Print ""
    Print "Arguments:"
    Print "  input.xml   - OpenXML file containing flowchart diagram"
    Print "  output.bas  - Output FreeBasic file (default: output.bas)"
    Print ""
    Print "Supported flowchart elements:"
    Print "  - Terminator (Start/End)"
    Print "  - Process (Rectangle)"
    Print "  - Decision (Diamond)"
    Print "  - Data (Parallelogram)"
    Print "  - Predefined Process (Double Rectangle)"
End Function

Sub Main()
    Dim inputFile As String
    Dim outputFile As String
    Dim xmlDoc As XMLDocument
    Dim flowchart As FlowchartDiagram
    Dim codeCtx As CodeContext
    Dim i As Integer
    
    '' Parse command line arguments
    If __FB_ARGC__ < 2 Then
        ShowUsage()
        Exit Sub
    End If
    
    inputFile = Command(1)
    
    If __FB_ARGC__ >= 3 Then
        outputFile = Command(2)
    Else
        outputFile = "output.bas"
    End If
    
    Print "OpenXML Flowchart to FreeBasic Converter"
    Print "======================================="
    Print ""
    Print "Input file: " & inputFile
    Print "Output file: " & outputFile
    Print ""
    
    '' Load and parse XML
    Print "Loading XML file..."
    xmlDoc = XMLLoadFile(inputFile)
    
    If xmlDoc.nodeCount = 0 Then
        Print "Error: Failed to parse XML file"
        Exit Sub
    End If
    
    Print "XML parsed successfully. Found " & xmlDoc.nodeCount & " nodes."
    Print ""
    
    '' Parse flowchart
    Print "Parsing flowchart..."
    flowchart = FlowchartParseFromXML(xmlDoc)
    
    If flowchart.shapeCount = 0 Then
        Print "Error: No flowchart shapes found"
        Exit Sub
    End If
    
    Print "Found " & flowchart.shapeCount & " shapes"
    
    '' Display flowchart information
    Print ""
    Print "Flowchart shapes:"
    Print "------------------"
    For i = 0 To flowchart.shapeCount - 1
        Print "  [" & (i + 1) & "] " & flowchart.shapes(i).shapeType & _
              " - " & flowchart.shapes(i).text
    Next i
    Print ""
    
    '' Validate flowchart
    If FlowchartValidate(flowchart) = 0 Then
        Print "Warning: Flowchart validation failed"
    End If
    
    '' Generate FreeBasic code
    Print "Generating FreeBasic code..."
    codeCtx = CodeGenerateFromFlowchart(flowchart)
    
    Print "Generated " & codeCtx.lineCount & " lines of code"
    Print ""
    
    '' Write to file
    CodeWriteToFile codeCtx, outputFile
    
    Print ""
    Print "Conversion complete!"
    Print ""
    
    '' Display generated code snippet
    Print "Generated code preview (first 10 lines):"
    Print "------------------------------------------"
    Dim previewLines As Integer
    previewLines = IIf(codeCtx.lineCount < 10, codeCtx.lineCount, 10)
    For i = 0 To previewLines - 1
        Print codeCtx.outputLines(i)
    Next i
    If codeCtx.lineCount > 10 Then
        Print "... (" & (codeCtx.lineCount - 10) & " more lines)"
    End If
End Sub

Main()
