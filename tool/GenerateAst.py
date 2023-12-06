import sys
import os

def defineAst(outputDir, baseName, types):
    path = os.path.join(outputDir, f"{baseName}.h")
    derivedClasses = []
    with open(path, "w") as writer:
        writer.write(f"class {baseName} {{\n")
        for type in types:
            className = type.split(":")[0].strip()
            fields = type.split(":")[1].strip()
            derivedClass = defineType(baseName, className, fields)
            derivedClasses.append(derivedClass)
        writer.write(f"}};\n")
        for derivedClass in derivedClasses:
            writer.write(f"{derivedClass}\n")
        writer.close()

def defineType(baseName, className, fieldList):
    writer = ""
    writer += f"class {className}: public {baseName} {{\n"
    #constructor
    writer += f"    {className}({fieldList}) {{\n"
    fields = fieldList.split(", ")
    for field in fields:
        name = field.split(" ")[1]
        writer += f"        this->{name} = {name};\n"
    writer += "    }\n"
    #fields
    writer += "\n"
    for field in fields:
        writer += f"    {field};\n"
    writer += f"}};\n"
    return writer

if(len(sys.argv) != 2):
    print("Usage: generate_ast <output directory>")
    exit(64)
outputDir = sys.argv[1]
defineAst(outputDir, "Expr", ["Binary   : Expr left, Token operator, Expr right",
                              "Grouping : Expr expression",
                              "Literal  : Object value",
                              "Unary    : Token operator, Expr right"])
