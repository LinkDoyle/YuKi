antlr_target(XMLGrammarLexer grammar/XMLLexer.g4
             LEXER PACKAGE antlrxmlparser)
antlr_target(XMLGrammarParser grammar/XMLParser.g4
             PARSER LISTENER VISITOR
             PACKAGE antlrxmlparser
             DEPENDS_ANTLR XMLGrammarLexer)

message (status "${ANTLR_XMLGrammarLexer_OUTPUT_DIR}")
message (status "${ANTLR_XMLGrammarParser_OUTPUT_DIR}")
message (status "${ANTLR_XMLGrammarLexer_CXX_OUTPUTS}")
message (status "${ANTLR_XMLGrammarParser_CXX_OUTPUTS}")

include_directories(${ANTLR4_INCLUDE_DIRS})
include_directories(${ANTLR_XMLGrammarLexer_OUTPUT_DIR})
include_directories(${ANTLR_XMLGrammarParser_OUTPUT_DIR})
source_group("antlr4cpp" FILES
  ${ANTLR_XMLGrammarLexer_CXX_OUTPUTS}
  ${ANTLR_XMLGrammarParser_CXX_OUTPUTS}
)
 
set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR}/src/experiment)
set(SOURCE_LIST
  "rpml_loader_test.cc"
)

add_executable(yuki_rpml_test ${SOURCE_LIST}
  ${ANTLR_XMLGrammarLexer_CXX_OUTPUTS}
  ${ANTLR_XMLGrammarParser_CXX_OUTPUTS}
)
set_target_properties(yuki_rpml_test PROPERTIES FOLDER "Testing")
target_link_libraries(yuki_rpml_test antlr4_static)
