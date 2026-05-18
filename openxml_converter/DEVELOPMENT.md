# Development Guide

## Architecture Overview

The OpenXML Flowchart to FreeBasic Converter is designed with a modular architecture:

```
OpenXMLConverter.bas (Main Entry Point)
    |
    ├── XMLParser.bas (XML Parsing)
    ├── FlowchartParser.bas (Flowchart Extraction)
    └── CodeGenerator.bas (Code Generation)
```

## Module Responsibilities

### XMLParser.bas
- Loads XML files from disk
- Parses XML into a node tree structure
- Provides attribute access and node searching
- Handles basic XML validation

**Key Types:**
```freebasic
Type XMLNode
    tagName As String          ' Name of XML tag
    content As String          ' Text content
    attributes As String       ' Raw attributes string
    parentIndex As Integer     ' Parent node index
    childrenCount As Integer   ' Number of children
End Type

Type XMLDocument
    nodes(0 To 9999) As XMLNode
    nodeCount As Integer
    rootIndex As Integer
End Type
```

### FlowchartParser.bas
- Identifies flowchart shapes in XML structure
- Extracts shape properties (type, position, text)
- Recognizes shape connections
- Validates flowchart structure

**Key Types:**
```freebasic
Type FlowchartShape
    shapeId As String          ' Unique identifier
    shapeType As String        ' Process, Decision, etc.
    x As Integer               ' X coordinate
    y As Integer               ' Y coordinate
    width As Integer           ' Shape width
    height As Integer          ' Shape height
    text As String             ' Label text
    connectedTo As String      ' Next shape ID
End Type

Type FlowchartDiagram
    shapes(0 To 999) As FlowchartShape
    shapeCount As Integer
End Type
```

### CodeGenerator.bas
- Traverses the flowchart structure
- Generates corresponding FreeBasic code
- Manages code formatting and indentation
- Handles output to file

**Key Types:**
```freebasic
Type CodeContext
    indentLevel As Integer     ' Current indentation level
    variableCounter As Integer ' Counter for generated variables
    labelCounter As Integer    ' Counter for labels
    outputLines(0 To 4999) As String
    lineCount As Integer
End Type
```

## Data Flow

1. **Input**: OOXML file (typically .xml extracted from .docx or .pptx)
2. **XMLParser**: Parse XML into node tree
3. **FlowchartParser**: Extract shapes and connections
4. **CodeGenerator**: Traverse flowchart and generate FreeBasic code
5. **Output**: .bas file with generated FreeBasic program

## Adding New Flowchart Shape Types

To support a new shape type:

1. **Identify the shape name** in OpenXML (e.g., "flowChartPredefinedProcess")

2. **Update FlowchartParser.bas**:
```freebasic
ElseIf InStr(typeAttr, "flowChartYourShape") > 0 Then
    shape.shapeType = "YourShape"
```

3. **Update CodeGenerator.bas** in `CodeTraverseFlow()` sub:
```freebasic
Case "YourShape"
    CodeAddLine ctx, "'' Your Shape: " & shape.text
    CodeAddLine ctx, "' Generated code for YourShape"
```

4. **Test** with example XML containing the new shape

## Adding New Code Generation Patterns

To generate different code patterns:

1. Create a new sub in CodeGenerator.bas:
```freebasic
Sub CodeGeneratePattern(ctx As CodeContext, shape As FlowchartShape)
    ' Your code generation logic
End Sub
```

2. Call it from `CodeTraverseFlow()` when appropriate shape is encountered

3. Use helper subs:
   - `CodeAddLine()` - Add a single line
   - `CodeAddIndent()` - Increase indentation
   - `CodeRemoveIndent()` - Decrease indentation

## Testing

### Unit Testing

Create test files in the repository:
```bash
test_xmlparser.bas      # Test XML parsing
test_flowchart.bas      # Test flowchart extraction
test_codegen.bas        # Test code generation
```

### Integration Testing

1. Create test OOXML files
2. Run converter
3. Verify generated FreeBasic code compiles
4. Execute generated program

### Example Test Case

```bash
# Create test input
cp examples/simple_flowchart.xml test_input.xml

# Run converter
./openxml_converter test_input.xml test_output.bas

# Compile generated code
fbc test_output.bas -o test_program

# Run and verify
./test_program
```

## Debugging Tips

### Enable Debug Output

Add debug output in XMLParser:
```freebasic
Print "DEBUG: Found node " & doc.nodes(i).tagName
```

### Inspect Intermediate Data

Print flowchart structure:
```freebasic
For i = 0 To flowchart.shapeCount - 1
    Print flowchart.shapes(i).shapeType & ": " & flowchart.shapes(i).text
Next i
```

### Examine Generated Code

Before writing to file:
```freebasic
For i = 0 To codeCtx.lineCount - 1
    Print codeCtx.outputLines(i)
Next i
```

## Performance Considerations

### Current Limits

- Maximum 10,000 XML nodes
- Maximum 1,000 flowchart shapes
- Maximum 5,000 lines of generated code

### Optimization Opportunities

1. **XML Parsing**: Current implementation is O(n)
   - Could use hash tables for faster lookup
   - Consider streaming parser for large files

2. **Shape Traversal**: Currently recursive
   - Could use explicit stack for deep flowcharts
   - Track visited nodes to avoid cycles

3. **Memory Usage**: Could implement:
   - Dynamic array resizing
   - Streaming code output
   - Incremental code generation

## FreeBasic Language Features Used

- **Type Definitions**: Custom data structures
- **String Operations**: InStr, Mid, Left, Right
- **File I/O**: Open, Print #, Line Input
- **Control Flow**: If/Then, Select/Case, Do/While
- **Functions and Subs**: Modular code organization
- **Comments**: Using single quote (') and double quote with code ("")

## Compiler Flags

### Compilation Options

```bash
# Standard compilation
fbc OpenXMLConverter.bas

# With optimizations
fbc -O2 OpenXMLConverter.bas

# With debug symbols
fbc -g OpenXMLConverter.bas

# Strict error checking
fbc -exx OpenXMLConverter.bas

# With warnings
fbc -w pedantic OpenXMLConverter.bas
```

## Future Enhancement Ideas

### Phase 2: Advanced Flow Control
- [ ] Loop detection and generation
- [ ] Conditional branching optimization
- [ ] Variable scope management
- [ ] Procedure/Function extraction

### Phase 3: Code Optimization
- [ ] Dead code removal
- [ ] Common subexpression elimination
- [ ] Variable lifetime analysis
- [ ] Code refactoring

### Phase 4: Extended Support
- [ ] Multiple diagram support
- [ ] Swimlane support
- [ ] Custom stencils
- [ ] Visio-specific features
- [ ] Code generation to other languages (C, Python)

## Contributing Guidelines

1. Follow existing code style
2. Add comments for complex logic
3. Update documentation
4. Test changes thoroughly
5. Create pull request with description

## Resources

- FreeBasic Manual: https://www.freebasic.net/wiki/DocToc
- ECMA-376 Standard: https://www.ecma-international.org/publications/standards/ecma-376.html
- DrawingML Spec: https://docs.microsoft.com/en-us/office/open-xml/drawingml-overview
