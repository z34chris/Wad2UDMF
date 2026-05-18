'' CodeGenerator.bas - Generate FreeBasic code from flowchart
'' Converts flowchart structures into executable FreeBasic programs

#ifndef __CODE_GENERATOR_BAS__
#define __CODE_GENERATOR_BAS__

#include "FlowchartParser.bas"

Type CodeContext
    indentLevel As Integer
    variableCounter As Integer
    labelCounter As Integer
    outputLines(0 To 4999) As String
    lineCount As Integer
End Type

'' Initialize code context
Function CodeContextInit() As CodeContext
    Dim ctx As CodeContext
    ctx.indentLevel = 0
    ctx.variableCounter = 0
    ctx.labelCounter = 0
    ctx.lineCount = 0
    CodeContextInit = ctx
End Function

'' Generate FreeBasic code from flowchart diagram
Function CodeGenerateFromFlowchart(diagram As FlowchartDiagram) As CodeContext
    Dim ctx As CodeContext
    
    ctx = CodeContextInit()
    
    '' Generate program header
    CodeAddLine ctx, "'' Auto-generated FreeBasic program from flowchart"
    CodeAddLine ctx, "'' Generated: " & Date
    CodeAddLine ctx, ""
    CodeAddLine ctx, "Declare Sub Main()"
    CodeAddLine ctx, ""
    CodeAddLine ctx, "Main()"
    CodeAddLine ctx, ""
    CodeAddLine ctx, "Sub Main()"
    ctx.indentLevel = ctx.indentLevel + 1
    
    '' Declare variables
    CodeGenerateVariableDeclarations ctx, diagram
    
    '' Generate logic
    CodeGenerateFlowLogic ctx, diagram
    
    ctx.indentLevel = ctx.indentLevel - 1
    CodeAddLine ctx, "End Sub"
    
    CodeGenerateFromFlowchart = ctx
End Function

'' Add line to code output
Sub CodeAddLine(ctx As CodeContext, line As String)
    Dim indent As String
    Dim i As Integer
    
    indent = ""
    For i = 1 To ctx.indentLevel * 4
        indent = indent & " "
    Next i
    
    If ctx.lineCount < 5000 Then
        ctx.outputLines(ctx.lineCount) = indent & line
        ctx.lineCount = ctx.lineCount + 1
    End If
End Sub

'' Generate variable declarations based on shapes
Sub CodeGenerateVariableDeclarations(ctx As CodeContext, diagram As FlowchartDiagram)
    Dim i As Integer
    Dim declaredVars As String
    
    declaredVars = ""
    
    '' Extract variable names from shape text
    For i = 0 To diagram.shapeCount - 1
        Dim varName As String
        varName = ExtractVariableFromText(diagram.shapes(i).text)
        If Len(varName) > 0 And InStr(declaredVars, varName) = 0 Then
            CodeAddLine ctx, "Dim " & varName & " As String"
            declaredVars = declaredVars & varName & ","
        End If
    Next i
    
    CodeAddLine ctx, ""
End Sub

'' Extract variable name from shape text (simple heuristic)
Function ExtractVariableFromText(text As String) As String
    Dim result As String
    Dim i As Integer
    Dim charVal As Integer
    
    result = ""
    
    '' Extract alphanumeric sequences
    For i = 1 To Len(text)
        charVal = Asc(Mid(text, i, 1))
        If (charVal >= 65 And charVal <= 90) Or _
           (charVal >= 97 And charVal <= 122) Or _
           (charVal >= 48 And charVal <= 57) Then
            result = result & Mid(text, i, 1)
        Else
            If Len(result) > 0 Then Exit For
        End If
    Next i
    
    ExtractVariableFromText = result
End Function

'' Generate main flow logic
Sub CodeGenerateFlowLogic(ctx As CodeContext, diagram As FlowchartDiagram)
    Dim i As Integer
    Dim currentShape As Integer
    
    '' Find starting shape (Terminator)
    currentShape = -1
    For i = 0 To diagram.shapeCount - 1
        If diagram.shapes(i).shapeType = "Terminator" Then
            currentShape = i
            Exit For
        End If
    Next i
    
    If currentShape = -1 Then currentShape = 0
    
    '' Generate flow traversal
    Dim visited(0 To 999) As Integer
    Dim visitCount As Integer
    visitCount = 0
    
    CodeTraverseFlow ctx, diagram, currentShape, visited(), visitCount
End Sub

'' Traverse and generate code for flowchart
Sub CodeTraverseFlow(ctx As CodeContext, diagram As FlowchartDiagram, shapeIndex As Integer, visited() As Integer, visitCount As Integer)
    Dim i As Integer
    Dim alreadyVisited As Integer
    Dim shape As FlowchartShape
    
    If shapeIndex < 0 Or shapeIndex >= diagram.shapeCount Then Exit Sub
    
    '' Check if already visited
    For i = 0 To visitCount - 1
        If visited(i) = shapeIndex Then
            alreadyVisited = 1
            Exit For
        End If
    Next i
    
    If alreadyVisited Then Exit Sub
    
    visited(visitCount) = shapeIndex
    visitCount = visitCount + 1
    
    shape = diagram.shapes(shapeIndex)
    
    '' Generate code based on shape type
    Select Case shape.shapeType
        Case "Terminator"
            CodeAddLine ctx, "'' Start/End: " & shape.text
            If InStr(UCase(shape.text), "END") > 0 Then
                CodeAddLine ctx, "Print \"Program completed\""
            Else
                CodeAddLine ctx, "Print \"Starting: " & shape.text & "\""
            End If
            
        Case "Process"
            CodeAddLine ctx, "'' Process: " & shape.text
            CodeAddLine ctx, "Print \"Executing: " & shape.text & "\""
            
        Case "Decision"
            CodeAddLine ctx, "'' Decision: " & shape.text
            CodeAddLine ctx, "If 1 Then"
            ctx.indentLevel = ctx.indentLevel + 1
            CodeAddLine ctx, "Print \"Condition: " & shape.text & "\""
            ctx.indentLevel = ctx.indentLevel - 1
            CodeAddLine ctx, "End If"
            
        Case "Data"
            CodeAddLine ctx, "'' Data Input/Output: " & shape.text
            CodeAddLine ctx, "Print \"Data: " & shape.text & "\""
            
        Case "PredefinedProcess"
            CodeAddLine ctx, "'' Predefined Process: " & shape.text
            CodeAddLine ctx, "Print \"Calling: " & shape.text & "\""
    End Select
    
    '' Continue to next connected shape
    If Len(shape.connectedTo) > 0 Then
        For i = 0 To diagram.shapeCount - 1
            If diagram.shapes(i).shapeId = shape.connectedTo Then
                CodeTraverseFlow ctx, diagram, i, visited(), visitCount
                Exit For
            End If
        Next i
    End If
End Sub

'' Write generated code to file
Sub CodeWriteToFile(ctx As CodeContext, filename As String)
    Dim fileHandle As Integer
    Dim i As Integer
    
    fileHandle = FreeFile()
    If Open(filename For Output As fileHandle) <> 0 Then
        Print "Error: Cannot create output file " & filename
        Exit Sub
    End If
    
    For i = 0 To ctx.lineCount - 1
        Print #fileHandle, ctx.outputLines(i)
    Next i
    
    Close fileHandle
    Print "Generated code written to: " & filename
End Sub

'' Get generated code as string
Function CodeGetAsString(ctx As CodeContext) As String
    Dim result As String
    Dim i As Integer
    
    result = ""
    For i = 0 To ctx.lineCount - 1
        result = result & ctx.outputLines(i) & Chr(10)
    Next i
    
    CodeGetAsString = result
End Function

#endif
