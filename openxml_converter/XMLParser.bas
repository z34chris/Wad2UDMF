'' XMLParser.bas - XML parsing utilities for OpenXML files
'' Handles parsing of Microsoft Office OpenXML drawing canvas files

#ifndef __XML_PARSER_BAS__
#define __XML_PARSER_BAS__

Type XMLNode
    tagName As String
    content As String
    attributes As String
    parentIndex As Integer
    childrenCount As Integer
End Type

Type XMLDocument
    nodes(0 To 9999) As XMLNode
    nodeCount As Integer
    rootIndex As Integer
End Type

'' Initialize XML document
Function XMLInitDocument() As XMLDocument
    Dim doc As XMLDocument
    doc.nodeCount = 0
    doc.rootIndex = -1
    XMLInitDocument = doc
End Function

'' Load and parse XML file
Function XMLLoadFile(filename As String) As XMLDocument
    Dim doc As XMLDocument
    Dim fileHandle As Integer
    Dim xmlContent As String
    Dim fileSize As LongInt
    
    doc = XMLInitDocument()
    
    fileHandle = FreeFile()
    If Open(filename For Input As fileHandle) <> 0 Then
        Print "Error: Cannot open file " & filename
        XMLLoadFile = doc
        Exit Function
    End If
    
    '' Read entire file
    xmlContent = ""
    While Not Eof(fileHandle)
        Dim line As String
        Line Input #fileHandle, line
        xmlContent = xmlContent & line & Chr(10)
    Wend
    Close fileHandle
    
    '' Parse XML content
    doc = XMLParseContent(xmlContent)
    XMLLoadFile = doc
End Function

'' Parse XML content string
Function XMLParseContent(content As String) As XMLDocument
    Dim doc As XMLDocument
    Dim pos As Integer
    Dim tagStart As Integer
    Dim tagEnd As Integer
    Dim currentNodeIndex As Integer
    
    doc = XMLInitDocument()
    currentNodeIndex = 0
    pos = 1
    
    While pos <= Len(content)
        tagStart = InStr(pos, content, "<")
        If tagStart = 0 Then Exit While
        
        tagEnd = InStr(tagStart, content, ">")
        If tagEnd = 0 Then Exit While
        
        Dim tagContent As String
        tagContent = Mid(content, tagStart + 1, tagEnd - tagStart - 1)
        
        '' Skip XML declaration and comments
        If Left(tagContent, 1) = "?" Or Left(tagContent, 1) = "!" Then
            pos = tagEnd + 1
            Continue While
        End If
        
        '' Handle closing tags
        If Left(tagContent, 1) = "/" Then
            pos = tagEnd + 1
            Continue While
        End If
        
        '' Parse opening tag
        If currentNodeIndex < 10000 Then
            doc.nodes(currentNodeIndex).tagName = XMLExtractTagName(tagContent)
            doc.nodes(currentNodeIndex).attributes = tagContent
            doc.nodes(currentNodeIndex).parentIndex = -1
            doc.nodes(currentNodeIndex).childrenCount = 0
            
            If doc.rootIndex = -1 Then
                doc.rootIndex = currentNodeIndex
            End If
            
            currentNodeIndex = currentNodeIndex + 1
            doc.nodeCount = currentNodeIndex
        End If
        
        pos = tagEnd + 1
    Wend
    
    XMLParseContent = doc
End Function

'' Extract tag name from tag content
Function XMLExtractTagName(tagContent As String) As String
    Dim spacePos As Integer
    Dim slashPos As Integer
    
    spacePos = InStr(1, tagContent, " ")
    slashPos = InStr(1, tagContent, "/")
    
    If spacePos > 0 And (slashPos = 0 Or spacePos < slashPos) Then
        XMLExtractTagName = Left(tagContent, spacePos - 1)
    ElseIf slashPos > 0 Then
        XMLExtractTagName = Left(tagContent, slashPos - 1)
    Else
        XMLExtractTagName = tagContent
    End If
End Function

'' Get attribute value from node
Function XMLGetAttribute(node As XMLNode, attrName As String) As String
    Dim searchStr As String
    Dim startPos As Integer
    Dim endPos As Integer
    Dim quoteStart As Integer
    Dim quoteEnd As Integer
    
    searchStr = attrName & "="
    startPos = InStr(1, node.attributes, searchStr)
    
    If startPos = 0 Then
        XMLGetAttribute = ""
        Exit Function
    End If
    
    startPos = startPos + Len(searchStr)
    quoteStart = InStr(startPos, node.attributes, Chr(34)) '' Double quote
    If quoteStart = 0 Then quoteStart = InStr(startPos, node.attributes, "'")
    
    If quoteStart = 0 Then
        XMLGetAttribute = ""
        Exit Function
    End If
    
    quoteEnd = InStr(quoteStart + 1, node.attributes, Chr(34))
    If quoteEnd = 0 Then quoteEnd = InStr(quoteStart + 1, node.attributes, "'")
    
    If quoteEnd > quoteStart Then
        XMLGetAttribute = Mid(node.attributes, quoteStart + 1, quoteEnd - quoteStart - 1)
    Else
        XMLGetAttribute = ""
    End If
End Function

'' Find node by tag name
Function XMLFindNode(doc As XMLDocument, tagName As String, startIndex As Integer = 0) As Integer
    Dim i As Integer
    For i = startIndex To doc.nodeCount - 1
        If doc.nodes(i).tagName = tagName Then
            XMLFindNode = i
            Exit Function
        End If
    Next i
    XMLFindNode = -1
End Function

#endif
