import sys
import os

def defineAst(outputDir, baseName, types):
    path = os.path.join(outputDir, f"{baseName[:-3]}.h")
    derivedClasses = []
    with open(path, "w") as writer:
        writer.write("#include \"Token.h\"\n")
        writer.write("// forward declarations\n")
        writer.write(f"template <typename R> class {baseName[:-3]};\n")
        for type in types:
            className = type.split(":")[0].strip()
            writer.write(f"template <typename R> class {className};\n")
        writer.write("template <typename R> class Visitor;\n");
        writer.write("\n")
        writer.write(f"template <typename R> class {baseName[:-3]} {{\n")
        str = "public:\n\tvirtual R accept(Visitor<R>* visitor) = 0;\n"
        writer.write(str.expandtabs(4))
        for type in types:
            className = type.split(":")[0].strip()
            fields = type.split(":")[1].strip()
            derivedClass = defineType(baseName, className, fields)
            derivedClasses.append(derivedClass)
        writer.write(f"}};\n\n")
        for derivedClass in derivedClasses:
            writer.write(f"{derivedClass}\n")
        defineVisitor(baseName, types, writer)
        writer.close()

def defineType(baseName, className, fieldList):
    writer = f"template <typename R> class {className}: public {baseName} {{\npublic:\n"
    #constructor
    fields = fieldList.split(", ")
    writer += f"\t{className}({fieldList}): " 
    for field in fields:
        name = field.split(" ")[1]
        writer += f"{name}({name}), "
    writer = writer[:-2]
    writer += f"\n\t{{}}\n\n"
    #fields
    writer += "\tR accept(Visitor<R>* visitor) override {\n"
    writer += f"\t\treturn visitor->visit{className}{baseName[:-3]}(this);\n"
    writer += "\t}\n\n"
    for field in fields:
        writer += f"\t{field};\n"
    writer += f"}};\n"
    return writer.expandtabs(4)

def defineVisitor(baseName, types, writer):
    #writer.write("template <typename R> class Visitor {\n")
    str = "template <typename R> class Visitor {\n"
    str += "public:\n"
    for type in types:
        typeName = type.split(":")[0].strip()
        str += f"\tvirtual R visit{typeName}{baseName[:-3]}({typeName}<R>* {baseName[:-3].lower()}) = 0;\n"

    str += "};"
    writer.write(str.expandtabs(4))

    

if(len(sys.argv) != 2):
    print("Usage: generate_ast <output directory>")
    exit(64)
outputDir = sys.argv[1]
defineAst(outputDir, "Expr<R>", ["Binary   : Expr<R>* left, Token* oprtr, Expr<R>* right",
                              "Grouping : Expr<R>* expression",
                              "Literal  : Token* value",
                              "Unary    : Token* oprtr, Expr<R>* right"])
