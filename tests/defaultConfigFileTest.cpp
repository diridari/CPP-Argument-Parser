//
// Created by basti on 01.05.2019.
//
#include <iostream>
#include <gtest/gtest.h>
#include <argvParser.h>

using namespace std;


bool a;
string b,c;
int DefaultConfCallBack1(int i, char *buff[]) {
    i++;
     a = true;
    return i;
}

int DefaultConfCallBack2(int i, char *buff[]) {
    i++;
    b = string(buff[i]);
    return i;
}

int DefaultConfCallBack3(int i, char *buff[]) {
    i++;
    c = string(buff[i]);
    return i;
}

TEST(defaultConfigFile, readSingeDefaultFile){
    argvParser parser("test");
    a = false;
    parser.addArg("-a", "-b", "test", DefaultConfCallBack1);
    parser.checkForDefaulConfigFilesIn("defaultConf","../tests/configFiles/");
    ASSERT_TRUE(parser.analyseArgv(0,NULL));
    ASSERT_TRUE(a);
}
TEST(defaultConfigFile, noFileExist){
    argvParser parser("test");
    a = false;
    parser.addArg("-a", "-b", "test", DefaultConfCallBack1);
    parser.checkForDefaulConfigFilesIn("defaultConfxx","../tests/configFiles/");
    ASSERT_TRUE(parser.analyseArgv(0,NULL));
    ASSERT_FALSE(a);
}

TEST(defaultConfigFile, TwoFilesOneSelected){
    argvParser parser("test");
    a = false;
    b = "";
    c = "";
    parser.addArg("-b", "-bb", "test", DefaultConfCallBack2);
    parser.addArg("-a", "aa", "test", DefaultConfCallBack3);
    parser.checkForDefaulConfigFilesIn("defaultConf","../tests/configFiles/");
    ASSERT_TRUE(parser.analyseArgv(0,NULL));
    ASSERT_EQ(b,"mainFolder");
    ASSERT_EQ(c,"");
}

TEST(defaultConfigFile, TwoFiles){
    argvParser parser("test");
    a = false;
    b = "";
    c = "";
    parser.addArg("-b", "-bb", "test", DefaultConfCallBack2);
    parser.addArg("-a", "aa", "test", DefaultConfCallBack3);
    parser.checkForDefaulConfigFilesIn("defaultConf"," ../tests/configFiles/ ../tests/configFiles/subFolder");
    ASSERT_TRUE(parser.analyseArgv(0,NULL));
    ASSERT_EQ(b,"mainFolder");
    ASSERT_EQ(c,"subFolder");
}

TEST(defaultConfigFile, OverwriteFiles){
    argvParser parser("test");
    a = false;
    b = "";
    c = "";
    parser.addArg("-b", "-a", "test", DefaultConfCallBack2);
    parser.checkForDefaulConfigFilesIn("defaultConf"," ../tests/configFiles/ ../tests/configFiles/subFolder");
    ASSERT_TRUE(parser.analyseArgv(0,NULL));
    ASSERT_EQ(b,"subFolder");
   // ASSERT_EQ(c,"subFolder");
}
TEST(defaultConfigFile, OverwriteFiles2){
    argvParser parser("test");
    a = false;
    b = "";
    c = "";
    parser.addArg("-b", "-a", "test", DefaultConfCallBack2);
    parser.checkForDefaulConfigFilesIn("defaultConf","../tests/configFiles/subFolder ../tests/configFiles/ ");
    ASSERT_TRUE(parser.analyseArgv(0,NULL));
    ASSERT_EQ(b,"mainFolder");
    // ASSERT_EQ(c,"subFolder");
}

TEST(defaultConfigFile, noLocations){
    argvParser parser("test");
    a = false;
    b = "";
    c = "";
    parser.addArg("-b", "-a", "test", DefaultConfCallBack1);
    parser.checkForDefaulConfigFilesIn("defaultConf","");
    ASSERT_TRUE(parser.analyseArgv(0,NULL));
    ASSERT_FALSE(a);
    // ASSERT_EQ(c,"subFolder");
}
