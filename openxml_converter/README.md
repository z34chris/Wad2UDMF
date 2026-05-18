# OpenXML Flowchart to FreeBasic Converter

A FreeBasic program that reads Microsoft Office OpenXML drawing canvas files and converts XML flowchart objects into executable FreeBasic programs.

## Overview

This converter allows you to:
- Parse flowchart diagrams from Office Open XML (OOXML) files
- Identify flowchart shapes (Process, Decision, Terminator, Data, etc.)
- Extract connections and flow logic
- Generate corresponding FreeBasic code that represents the flowchart logic

## Features

- **XML Parsing**: Robust XML parsing for OpenXML format
- **Flowchart Recognition**: Identifies standard flowchart shapes
- **Code Generation**: Converts flowchart logic into FreeBasic source code
- **Error Handling**: Validates flowchart structure and reports issues
- **Extensible Design**: Modular architecture for easy enhancement

## Supported Flowchart Elements

| Flowchart Shape | FreeBasic Generation |
|-----------------|----------------------|
| Terminator | Start/End comments |
| Process | Print/action statements |
| Decision | If/Then conditions |
| Data | Input/Output operations |
| Predefined Process | Function/Sub calls |

## File Structure

```
openxml_converter/
├── XMLParser.bas           # XML parsing utilities
├── FlowchartParser.bas     # Flowchart shape parsing
├── CodeGenerator.bas       # FreeBasic code generation
├── OpenXMLConverter.bas    # Main program
├── README.md              # This file
└── examples/              # Example OOXML files
    └── simple_flowchart.xml
```

## Building the Program

### Requirements
- FreeBasic compiler (version 1.0 or later)
- Microsoft Office OpenXML drawing files (from Word/Visio)

### Compilation

```bash
fbc OpenXMLConverter.bas -o openxml_converter
```

On Windows:
```bash
fbc OpenXMLConverter.bas -o openxml_converter.exe
```

## Usage

### Basic Usage

```bash
./openxml_converter input.xml output.bas
```

### Command Line Options

```
OpenXML Flowchart to FreeBasic Converter
Usage: OpenXMLConverter <input.xml> [output.bas]

Arguments:
  input.xml   - OpenXML file containing flowchart diagram
  output.bas  - Output FreeBasic file (default: output.bas)
```

### Example

```bash
# Convert a flowchart from Word document
./openxml_converter flowchart.xml myprogram.bas

# Compile the generated FreeBasic code
fbc myprogram.bas -o myprogram

# Run the generated program
./myprogram
```

## How to Extract OOXML from Office Documents

### From Microsoft Word (.docx)

1. Save your Word document as `.docx`
2. Rename the file to `.zip`
3. Extract the archive
4. Navigate to `word/document.xml` for text shapes
5. For drawing objects, check `word/drawings/*.xml`

### From Microsoft Visio

1. Save diagram as `.vsdx`
2. Rename to `.zip` and extract
3. Find drawing XML in `visio/drawings/` or `customXml/`

## Module Documentation

### XMLParser.bas

Provides basic XML parsing functionality:
- `XMLLoadFile()` - Load and parse XML from file
- `XMLParseContent()` - Parse XML string content
- `XMLFindNode()` - Search for nodes by tag name
- `XMLGetAttribute()` - Extract attribute values
- `XMLExtractTagName()` - Parse tag names

### FlowchartParser.bas

Parses flowchart structures from XML:
- `FlowchartParseFromXML()` - Extract shapes from XML document
- `FlowchartParseShape()` - Parse individual shape properties
- `FlowchartValidate()` - Validate flowchart structure
- `XMLExtractTextFromShape()` - Extract text labels

### CodeGenerator.bas

Generates FreeBasic code:
- `CodeGenerateFromFlowchart()` - Main code generation function
- `CodeTraverseFlow()` - Traverse flowchart logic
- `CodeGenerateVariableDeclarations()` - Generate variable declarations
- `CodeWriteToFile()` - Write generated code to file
- `CodeGetAsString()` - Get generated code as string

## Example Input/Output

### Input XML Structure (Simplified)

```xml
<?xml version="1.0"?>
<p:sld xmlns:p="http://schemas.openxmlformats.org/presentationml/2006/main">
  <p:cSld>
    <p:spTree>
      <!-- Terminator Shape -->
      <p:sp>
        <p:nvSpPr>
          <p:cNvPr id="1" name="Start"/>
        </p:nvSpPr>
        <p:spPr><a:prstGeom prst="flowChartTerminator"/></p:spPr>
        <p:txBody><a:p><a:t>START</a:t></a:p></p:txBody>
      </p:sp>
      
      <!-- Process Shape -->
      <p:sp>
        <p:nvSpPr>
          <p:cNvPr id="2" name="Process1"/>
        </p:nvSpPr>
        <p:spPr><a:prstGeom prst="flowChartProcess"/></p:spPr>
        <p:txBody><a:p><a:t>Do Something</a:t></a:p></p:txBody>
      </p:sp>
    </p:spTree>
  </p:cSld>
</p:sld>
```

### Generated FreeBasic Code

```freebasic
'' Auto-generated FreeBasic program from flowchart
'' Generated: 05/18/2026

Declare Sub Main()

Main()

Sub Main()
    '' Start/End: START
    Print "Starting: START"
    
    '' Process: Do Something
    Print "Executing: Do Something"
    
    '' Start/End: END
    Print "Program completed"
End Sub
```

## Limitations and Future Enhancements

### Current Limitations

- Basic shape type detection
- Linear flow parsing (limited loop/branch support)
- Simple text extraction
- No formatting preservation

### Planned Enhancements

- [ ] Advanced control flow (loops, branches)
- [ ] Variable extraction and type inference
- [ ] Function/procedure generation
- [ ] Complex shape connections
- [ ] Code optimization
- [ ] Comments and documentation preservation
- [ ] Support for multiple diagrams
- [ ] Integration with Visio stencils

## Troubleshooting

### "Error: Cannot open file"
- Verify the input file path is correct
- Ensure the file has read permissions
- Confirm it's a valid OOXML/XML file

### "No flowchart shapes found"
- The XML may not contain standard flowchart elements
- Ensure shapes use proper namespace (DrawingML)
- Check that shapes have the correct geometry types

### Generated code doesn't compile
- Review the generated FreeBasic code for syntax errors
- Ensure FreeBasic version compatibility
- Check for special characters in shape text

## Resources

- [FreeBasic Documentation](https://www.freebasic.net/wiki/DocToc)
- [OOXML Specification](https://www.ecma-international.org/publications/standards/ecma-376.html)
- [DrawingML Reference](https://docs.microsoft.com/en-us/office/open-xml/drawingml-flowchart)
- [Flowchart Symbols](https://en.wikipedia.org/wiki/Flowchart#Symbols)

## License

This project is part of the Wad2UDMF repository.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests for:
- Bug fixes
- New flowchart shape types
- Improved code generation
- Better XML parsing
- Documentation improvements

## Author

z34chris

## Version

1.0.0 - Initial release
