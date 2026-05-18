'' FlowchartParser.bas - Parse flowchart objects from OpenXML
'' Identifies flowchart shapes and connections

#ifndef __FLOWCHART_PARSER_BAS__
#define __FLOWCHART_PARSER_BAS__

#include "XMLParser.bas"

Type FlowchartShape
    shapeId As String
    shapeType As String '' Process, Decision, Terminator, Data
    x As Integer
    y As Integer
    width As Integer
    height As Integer
    text As String
    connectedTo As String '' Target shape ID
End Type

Type FlowchartDiagram
    shapes(0 To 999) As FlowchartShape
    shapeCount As Integer
End Type

'' Initialize flowchart diagram
Function FlowchartInitDiagram() As FlowchartDiagram
    Dim diagram As FlowchartDiagram
    diagram.shapeCount = 0
    FlowchartInitDiagram = diagram
End Function

'' Parse flowchart from OpenXML document
Function FlowchartParseFromXML(doc As XMLDocument) As FlowchartDiagram
    Dim diagram As FlowchartDiagram
    Dim i As Integer
    
    diagram = FlowchartInitDiagram()
    
    '' Search for shape definitions in drawing ML namespace
    For i = 0 To doc.nodeCount - 1
        If InStr(doc.nodes(i).tagName, "sp") > 0 Or InStr(doc.nodes(i).tagName, "shape") > 0 Then
            If diagram.shapeCount < 1000 Then
                FlowchartParseShape doc, doc.nodes(i), diagram.shapes(diagram.shapeCount)
                diagram.shapeCount = diagram.shapeCount + 1
            End If
        End If
    Next i
    
    FlowchartParseFromXML = diagram
End Function

'' Parse individual shape
Sub FlowchartParseShape(doc As XMLDocument, node As XMLNode, shape As FlowchartShape)
    Dim nameAttr As String
    Dim typeAttr As String
    
    shape.shapeId = XMLGetAttribute(node, "id")
    
    '' Determine shape type from preset geometry or type attribute
    typeAttr = XMLGetAttribute(node, "type")
    
    If InStr(typeAttr, "flowChartProcess") > 0 Then
        shape.shapeType = "Process"
    ElseIf InStr(typeAttr, "flowChartDecision") > 0 Then
        shape.shapeType = "Decision"
    ElseIf InStr(typeAttr, "flowChartTerminator") > 0 Then
        shape.shapeType = "Terminator"
    ElseIf InStr(typeAttr, "flowChartData") > 0 Then
        shape.shapeType = "Data"
    ElseIf InStr(typeAttr, "flowChartPredefinedProcess") > 0 Then
        shape.shapeType = "PredefinedProcess"
    Else
        shape.shapeType = "Process"
    End If
    
    '' Parse position and size
    shape.x = Val(XMLGetAttribute(node, "x"))
    shape.y = Val(XMLGetAttribute(node, "y"))
    shape.width = Val(XMLGetAttribute(node, "cx"))
    shape.height = Val(XMLGetAttribute(node, "cy"))
    
    '' Extract text content
    shape.text = XMLExtractTextFromShape(doc, node)
    
    '' Parse connections (simplified - look for connector references)
    shape.connectedTo = XMLGetAttribute(node, "nextId")
End Sub

'' Extract text content from shape
Function XMLExtractTextFromShape(doc As XMLDocument, shapeNode As XMLNode) As String
    Dim result As String
    Dim i As Integer
    
    result = ""
    
    '' Look for text elements within shape
    For i = 0 To doc.nodeCount - 1
        If (doc.nodes(i).tagName = "a:t" Or doc.nodes(i).tagName = "t") Then
            If Len(result) = 0 Then
                '' In real implementation, extract from doc.nodes(i).content
                result = doc.nodes(i).content
            End If
        End If
    Next i
    
    XMLExtractTextFromShape = result
End Function

'' Validate flowchart structure
Function FlowchartValidate(diagram As FlowchartDiagram) As Integer
    If diagram.shapeCount = 0 Then
        Print "Error: No shapes found in diagram"
        FlowchartValidate = 0
        Exit Function
    End If
    
    '' Check for terminator shapes
    Dim hasStart As Integer
    Dim hasEnd As Integer
    Dim i As Integer
    
    For i = 0 To diagram.shapeCount - 1
        If diagram.shapes(i).shapeType = "Terminator" Then
            hasStart = 1
        End If
    Next i
    
    FlowchartValidate = 1
End Function

#endif
