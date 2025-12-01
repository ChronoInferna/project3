// #include <catch2/catch_test_macros.hpp>
// #include <string>
// #include <vector>
//
// #include "../src/Compass.hpp"
//
// TEST_CASE("insert: successful insertion and duplicate UFID detection") {
//     Compass compass("data/edges.csv", ',', "data/classes.csv", ',');
//
//     std::vector<std::string> commands = {
//         "insert \"Alice A\" 00000001 10 2 COP3530 COT3100",  // Valid
//         "insert \"Bob B\" 00000002 11 1 COP3530",            // Valid
//         "insert \"Evil\" 00000001 12 1 COP3502"  // Duplicate UFID should
//         fail
//     };
//
//     std::vector<std::string> outputs;
//     for (const auto& cmd : commands) {
//         std::string out = compass.parseInput(cmd);
//         outputs.push_back(out);
//     }
//
//     // Test individual outputs
//     REQUIRE(outputs[0].find("successful") !=
//             std::string::npos);  // Alice insert
//     REQUIRE(outputs[1].find("successful") != std::string::npos);  // Bob
//     insert REQUIRE(outputs[2].find("unsuccessful") !=
//             std::string::npos);  // Evil insert (duplicate UFID)
// }
//
// TEST_CASE("insert: invalid class count (N mismatch) is unsuccessful") {
//     Compass compass("data/edges.csv", ',', "data/classes.csv", ',');
//
//     std::vector<std::string> commands = {
//         "insert \"Charlie C\" 00000003 12 2 COP3502"  // N=2 but only one
//         class
//                                                       // provided -> invalid
//     };
//
//     std::vector<std::string> outputs;
//     for (const auto& cmd : commands) {
//         std::string out = compass.parseInput(cmd);
//         outputs.push_back(out);
//     }
//
//     REQUIRE(outputs[0].find("unsuccessful") != std::string::npos);
// }
//
// TEST_CASE("remove: successful and unsuccessful removal") {
//     Compass compass("data/edges.csv", ',', "data/classes.csv", ',');
//
//     std::vector<std::string> commands = {
//         "insert \"Dana D\" 00000004 20 2 COP3502 COP3530",
//         "remove 00000004",  // Should succeed
//         "remove 00000004"   // Now should fail
//     };
//
//     std::vector<std::string> outputs;
//     for (const auto& cmd : commands) {
//         std::string out = compass.parseInput(cmd);
//         outputs.push_back(out);
//     }
//
//     REQUIRE(outputs.size() >= 3);
//     REQUIRE(outputs[0].find("successful") !=
//             std::string::npos);  // Insert succeeded
//     REQUIRE(outputs[1].find("successful") !=
//             std::string::npos);  // Removal succeeded
//     REQUIRE(outputs[2].find("unsuccessful") !=
//             std::string::npos);  // Second removal fails
// }
//
// TEST_CASE(
//     "dropClass: successful drop and auto-remove when last class removed") {
//     Compass compass("data/edges.csv", ',', "data/classes.csv", ',');
//
//     std::vector<std::string> commands = {
//         "insert \"Evan E\" 00000005 30 2 COP3502 COP3530",
//         "dropClass 00000005 COP3502",  // Student still has 1 class
//         "dropClass 00000005 COP3530",  // Last class removed -> student
//         dropped "dropClass 00000005 COP3502"   // Further drop should fail
//     };
//
//     std::vector<std::string> outputs;
//     for (const auto& cmd : commands) {
//         std::string out = compass.parseInput(cmd);
//         outputs.push_back(out);
//     }
//
//     REQUIRE(outputs.size() >= 4);
//     REQUIRE(outputs[0].find("successful") !=
//             std::string::npos);  // Insert succeeded
//     REQUIRE(outputs[1].find("successful") !=
//             std::string::npos);  // First dropClass succeeded
//     REQUIRE(outputs[2].find("successful") !=
//             std::string::npos);  // Second dropClass succeeded
//     REQUIRE(outputs[3].find("unsuccessful") !=
//             std::string::npos);  // Third dropClass failed
// }
//
// TEST_CASE(
//     "replaceClass: success, and failure cases (no student / lacking old / "
//     "already has new)") {
//     Compass compass("data/edges.csv", ',', "data/classes.csv", ',');
//
//     std::vector<std::string> commands = {
//         "insert \"Fiona F\" 00000006 40 2 COP3502 COP3530",
//         "replaceClass 00000006 COP3502 COP3100",  // Successful replace
//         "replaceClass 00000006 COP3502 COP3100",  // Attempt replace when
//                                                   // student does not have
//                                                   // CLASSCODE_1
//         "insert \"Gina G\" 00000007 41 2 COP3100 COP3530",
//         "replaceClass 00000007 COP3100 COP3530",  // Should fail (already has
//                                                   // new)
//         "replaceClass 99999999 COP3502 COT3100"   // Non-existent student
//     };
//
//     std::vector<std::string> outputs;
//     for (const auto& cmd : commands) {
//         std::string out = compass.parseInput(cmd);
//         outputs.push_back(out);
//     }
//
//     REQUIRE(outputs.size() >= 6);
//     REQUIRE(outputs[0].find("successful") !=
//             std::string::npos);  // Fiona insert
//     REQUIRE(outputs[1].find("successful") !=
//             std::string::npos);  // replaceClass success
//     REQUIRE(outputs[2].find("unsuccessful") !=
//             std::string::npos);  // Missing old class
//     REQUIRE(outputs[3].find("successful") != std::string::npos);  // Gina
//     insert REQUIRE(outputs[4].find("unsuccessful") !=
//             std::string::npos);  // Already has new class
//     REQUIRE(outputs[5].find("unsuccessful") !=
//             std::string::npos);  // Non-existent student
// }
//
// TEST_CASE(
//     "removeClass: removes class from all students and prints count; "
//     "auto-deletes students left with 0 classes") {
//     Compass compass("data/edges.csv", ',', "data/classes.csv", ',');
//
//     std::vector<std::string> commands = {
//         "insert \"Hank\" 00000008 50 1 COP3530",
//         "insert \"Ivy\" 00000009 51 2 COP3530 COP3502",
//         "removeClass COP3530",  // Remove COP9999 -> affected students = 2
//         "removeClass COP3502"   // Remove COP3502 -> affected students = 1
//     };
//
//     std::vector<std::string> outputs;
//     for (const auto& cmd : commands) {
//         std::string out = compass.parseInput(cmd);
//         outputs.push_back(out);
//     }
//
//     REQUIRE(outputs.size() >= 4);
//     REQUIRE(outputs[0].find("successful") != std::string::npos);  // Hank
//     insert REQUIRE(outputs[1].find("successful") != std::string::npos);  //
//     Ivy insert REQUIRE(outputs[2].find("2") !=
//             std::string::npos);  // removeClass COP9999 affects 2 students
//     REQUIRE(outputs[3].find("1") !=
//             std::string::npos);  // removeClass COP3502 affects 1 student
// }
//
// TEST_CASE(
//     "printShortestEdges: formatting, unreachable class -> -1, lexicographic "
//     "sorting of classes") {
//     Compass compass("data/edges.csv", ',', "data/classes.csv", ',');
//
//     std::vector<std::string> commands = {
//         "insert \"Kyle\" 00000010 52 3 COP3530 COP4600 COT3100",
//         "insert \"Joe\" 00000020 21 3 COP3530 COP4600 COT3100",
//         "printShortestEdges 00000010", "printShortestEdges 00000020"};
//
//     std::vector<std::string> outputs;
//     for (const auto& cmd : commands) {
//         std::string out = compass.parseInput(cmd);
//         outputs.push_back(out);
//     }
//
//     REQUIRE(outputs.size() >= 4);
//     REQUIRE(outputs[0].find("successful") !=
//             std::string::npos);  // First insert succeeded
//     REQUIRE(outputs[1].find("successful") !=
//             std::string::npos);  // Second insert succeeded
//
//     // Check printShortestEdges output format
//     std::string kyleOutput = outputs[2];
//     REQUIRE(kyleOutput.find("Name: Kyle") != std::string::npos);
//     REQUIRE(kyleOutput.find("Total Time: -1") != std::string::npos);
//
//     std::string joeOutput = outputs[3];
//     REQUIRE(joeOutput.find("Name: Joe") != std::string::npos);
//     REQUIRE(joeOutput.find("Total Time:") != std::string::npos);
//     // Lexicographic order: COP3530, COP4600, COT3100
//     size_t posA = joeOutput.find("COP3530");
//     size_t posB = joeOutput.find("COP4600");
//     size_t posC = joeOutput.find("COT3100");
//     REQUIRE(posA != std::string::npos);
//     REQUIRE(posB != std::string::npos);
//     REQUIRE(posC != std::string::npos);
//     REQUIRE(posA < posB);
//     REQUIRE(posB < posC);
// }
//
// TEST_CASE("printStudentZone: prints MST cost line with correct prefix") {
//     Compass compass("data/edges.csv", ',', "data/classes.csv", ',');
//
//     std::vector<std::string> commands = {
//         "insert \"Liam\" 00000011 21 2 COP3530 COP3502",
//         "printStudentZone 00000011"};
//
//     std::vector<std::string> outputs;
//     for (const auto& cmd : commands) {
//         std::string out = compass.parseInput(cmd);
//         outputs.push_back(out);
//     }
//
//     REQUIRE(outputs.size() >= 2);
//     REQUIRE(outputs[0].find("successful") !=
//             std::string::npos);  // Insert succeeded
//
//     // Check printStudentZone output format
//     std::string printOutput = outputs[1];
//     REQUIRE(printOutput.find("Student Zone Cost For Liam:") !=
//             std::string::npos);
//     // Cost should be an integer following colon (too lazy to calculate)
//     size_t pos = printOutput.find("Student Zone Cost For Liam:");
//     REQUIRE(pos != std::string::npos);
//     std::string tail = printOutput.substr(pos);
//     REQUIRE(tail.find_first_of("0123456789") != std::string::npos);
// }
//
// TEST_CASE("incorrect commands: at least five malformed or invalid inputs") {
//     Compass compass("data/edges.csv", ',', "data/classes.csv", ',');
//
//     std::vector<std::string> bad = {
//         "insert A B C",  // Missing quotes, missing fields
//         "insert \"BadName\" 1234567A 10 1 COP3530",  // Bad UFID characters
//         "insert \"BadName\" 00000013 X 1 COP3530",   // Bad residence ID
//         "remove",                                    // Missing UFID
//         "dropClass 00000022",                        // Missing CLASSCODE
//         "replaceClass 00000022 ONLYONE",             // Too few args
//         "printShortestEdges",                        // Missing ID
//     };
//
//     for (auto& cmd : bad) {
//         std::string out = compass.parseInput(cmd);
//         REQUIRE(out.find("unsuccessful") != std::string::npos);
//     }
// }
//
// TEST_CASE(
//     "edge cases: remove nonexistent student, drop nonexistent class, replace
//     " "with nonexistent class") { Compass compass("data/edges.csv", ',',
//     "data/classes.csv", ',');
//
//     compass.parseInput("insert \"Zee\" 00000091 10 1 COP3530");
//
//     std::string out1 = compass.parseInput("remove 99999999");
//     REQUIRE(out1.find("unsuccessful") != std::string::npos);
//
//     std::string out2 = compass.parseInput("dropClass 00000091 XXXXXX");
//     REQUIRE(out2.find("unsuccessful") != std::string::npos);
//
//     std::string out3 =
//         compass.parseInput("replaceClass 00000091 COP3530 XXXX000");
//     REQUIRE(out3.find("unsuccessful") != std::string::npos);
// }
//
// TEST_CASE(
//     "dropClass / removeClass / replaceClass / remove: additional correctness
//     " "tests") { Compass compass("data/edges.csv", ',', "data/classes.csv",
//     ',');
//
//     REQUIRE(compass.parseInput("insert \"Test\" 00000030 23 2 COP3502
//     COP3530")
//                 .find("successful") != std::string::npos);
//
//     // Replace existing class
//     REQUIRE(compass.parseInput("replaceClass 00000030 COP3502 COT3100")
//                 .find("successful") != std::string::npos);
//
//     // Drop one, not last
//     REQUIRE(
//         compass.parseInput("dropClass 00000030 COT3100").find("successful")
//         != std::string::npos);
//
//     // Remove class from all students
//     std::string out = compass.parseInput("removeClass COP3530");
//     REQUIRE(out.find("1") != std::string::npos);
//
//     // Now student has 0 classes → auto removed
//     std::string out2 = compass.parseInput("remove 00000030");
//     REQUIRE(out2.find("unsuccessful") != std::string::npos);
// }
//
// TEST_CASE(
//     "printShortestEdges: reachable, then unreachable after toggling edges") {
//     Compass compass("data/edges.csv", ',', "data/classes.csv", ',');
//
//     // Insert student living at 1, class at 14
//     REQUIRE(compass.parseInput("insert \"Walker\" 00000040 1 1 COP3530")
//                 .find("successful") != std::string::npos);
//
//     // Should be reachable initially
//     std::string outA = compass.parseInput("printShortestEdges 00000040");
//     REQUIRE(outA.find("Total Time: -1") == std::string::npos);
//
//     // Toggle the edges from 1 off
//     REQUIRE(compass.parseInput("toggleEdgesClosure 3 1 2 1 4 1 50")
//                 .find("successful") != std::string::npos);
//
//     // Now unreachable
//     std::string outB = compass.parseInput("printShortestEdges 00000040");
//     REQUIRE(outB.find("Total Time: -1") != std::string::npos);
//
//     // Toggle edge back on; should become reachable again
//     REQUIRE(compass.parseInput("toggleEdgesClosure 3 1 2 1 4 1 50")
//                 .find("successful") != std::string::npos);
//
//     std::string outC = compass.parseInput("printShortestEdges 00000040");
//     REQUIRE(outC.find("Total Time: -1") == std::string::npos);
// }
//
// // TEST_CASE(
// //     "verifySchedule (extra credit): single-class -> unsuccessful;
// multi-class
// //     " "prints feasibility between consecutive classes") { Compass
// //     compass("data/edges.csv", ',', "data/classes.csv", ','); std::string
// //     cmds;
// //     // single-class student
// //     cmds += "insert \"Mia\" 00000012 300 1 COP1\n";
// //     cmds += "verifySchedule 00000012\n";
// //     // multi-class student: We supply two classes and assume the program
// uses
// //     // stored class times
// //     cmds += "insert \"Noah\" 00000013 301 2 COPA COPB\n";
// //     cmds += "verifySchedule 00000013\n";
// //
// //     std::string out = compass.parseInput(cmds);
// //
// //     // Ensure single-class returns unsuccessful
// //     REQUIRE(out.find("unsuccessful") != std::string::npos);
// //
// //     // For the multi-class check, ensure header and at least one "Can make
// //     it!"
// //     // or "Cannot make it!" line
// //     REQUIRE(out.find("Schedule Check for Noah:") != std::string::npos);
// //     REQUIRE(out.find("Can make it!") != std::string::npos ||
// //             out.find("Cannot make it!") != std::string::npos);
// // }
