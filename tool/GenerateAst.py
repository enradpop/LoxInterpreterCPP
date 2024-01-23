import sys
import os
from enum import Enum

class TokenType(Enum):
    TOKEN = 1
    EXPR = 2

def defineAst(outputDir, baseName, types):
    path = os.path.join(outputDir, f"{baseName[:-3]}.h")
    derivedClasses = []
    with open(path, "w") as writer:
        writer.write("#include <memory>\n")
        writer.write("#include \"Token.h\"\n")
        writer.write("// forward declarations\n")
        writer.write(f"template <typename R> class {baseName[:-3]};\n")
        for type in types:
            className = type.split(":")[0].strip()
            writer.write(f"template <typename R> class {className};\n")
        writer.write("template <typename R> class Visitor;\n");
        writer.write("\n")
        writer.write(f"template <typename R> class {baseName[:-3]} {{\n")
        str = "public:\n\tvirtual R accept(Visitor<R> const& visitor) = 0;\n"
        str += "\tvirtual ~Expr(){};\n"
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
    writer += f"\t{className}(" 
    for field in fields:
        type, name = getTypeAndName(field)
        if whatType(type) == TokenType.TOKEN:
            type = f"{type} const&"
        else:
            type = f"{type}*"
        writer += f"{type} {name}, "
    writer = writer[:-2]
    writer += f"): "
    for field in fields:
        type, name = getTypeAndName(field)
        writer += f"{name}({name}), "
    writer = writer[:-2]
    writer += f"\n\t{{}}\n\n"
    #destructor
    writer += f"\t~{className}() override {{}}\n\n"
    #fields
    writer += "\tR accept(Visitor<R> const& visitor) override {\n"
    writer += f"\t\treturn visitor.visit{className}{baseName[:-3]}(*this);\n"
    writer += "\t}\n\n"
    for field in fields:
        type, name = getTypeAndName(field)
        if whatType(type) == TokenType.EXPR:
            type = f"std::unique_ptr<{type}>"
        writer += f"\t{type} {name};\n"
    writer += f"}};\n"
    return writer.expandtabs(4)

def getTypeAndName(field):
    type = field.split(" ")[0]
    name = field.split(" ")[1]
    return type, name
def whatType(type):
    if(type == "Token"):
        return TokenType.TOKEN
    return TokenType.EXPR

def defineVisitor(baseName, types, writer):
    #writer.write("template <typename R> class Visitor {\n")
    str = "template <typename R> class Visitor {\n"
    str += "public:\n"
    for type in types:
        typeName = type.split(":")[0].strip()
        str += f"\tvirtual R visit{typeName}{baseName[:-3]}({typeName}<R>& {baseName[:-3].lower()}) const = 0;\n"

    str += "};"
    writer.write(str.expandtabs(4))

    

if(len(sys.argv) != 2):
    print("Usage: generate_ast <output directory>")
    exit(64)
outputDir = sys.argv[1]
defineAst(outputDir, "Expr<R>", ["Binary   : Expr<R> left, Token oprtr, Expr<R> right",
                              "Grouping : Expr<R> expression",
                              "Literal  : Token value",
                              "Unary    : Token oprtr, Expr<R> right"])
