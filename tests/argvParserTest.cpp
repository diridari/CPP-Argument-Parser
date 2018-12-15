//
// Created by basto on 4/15/18.
//

#include <gtest/gtest.h>
#include "../include/argvParser.h"

string port_ = "";
bool d = false;
int callBackPortTestMeth(int i,char *buff[]){
    i++;
    port_ = buff[i];
    return i;
}
int callBackD(int i,char *buff[]){
    d = true;
    return i;
}

TEST(argvParser, CheckRequiredConfigFail1){
    argvParser parser("test");
    parser.addArg("t", "tt", "test", callBackD, 0, true);
    char *arg[] = {"program", };
    ASSERT_FALSE(parser.analyseArgv(1,arg));
}

TEST(argvParser, CheckRequiredConfigFail2){
    argvParser parser("test");
    parser.addArg("t", "tt", "test", callBackD, 0, true);
    parser.addArg("x", "xx", "test", callBackD, 0, false);
    char *arg[] = {"program","x" };
    ASSERT_FALSE(parser.analyseArgv(2,arg));

}

TEST(argvParser, CheckRequiredConfigFail3){
    argvParser parser("test");
    parser.addArg("t", "tt", "test", callBackD, 0, true);
    parser.addArg("x", "xx", "test", callBackD, 0, true);
    char *arg[] = {"program","x" };
    ASSERT_FALSE(parser.analyseArgv(2,arg));
}

TEST(argvParser, CheckRequiredConfigFail4){
    argvParser parser("test");
    parser.addArg("t", "tt", "test", callBackD, 0, true);
    parser.addArg("x", "xx", "test", callBackD, 0, true);
    char *arg[] = {"program","t" };
    ASSERT_FALSE(parser.analyseArgv(2,arg));
}

TEST(argvParser, CheckRequiredConfigFail5){
    argvParser parser("test");
    parser.addArg("t", "tt", "test", callBackD, 0, false);
    parser.addArg("x", "xx", "test", callBackD, 0, true);
    char *arg[] = {"program","t" };
    ASSERT_FALSE(parser.analyseArgv(2,arg));
}

TEST(argvParser, CheckRequiredConfigFail6){
    argvParser parser("test");
    parser.addArg("t", "tt", "test", callBackD, 0, true);
    parser.addArg("x", "xx", "test", callBackD, 0, true);
    char *arg[] = {"program","t","xx" };
    ASSERT_TRUE(parser.analyseArgv(3,arg));
}

TEST(argvParser, CheckRequiredConfigFail7){
    argvParser parser("test");
    parser.addArg("t", "tt", "test", callBackD, 0, true);
    parser.addArg("x", "xx", "test", callBackD, 0, true);
    parser.addArg("a", "aa", "test", callBackD, 0, false);
    char *arg[] = {"program","t","xx" };
    ASSERT_TRUE(parser.analyseArgv(3,arg));
}

TEST(argvParser, CheckRequiredConfigFail8){
    argvParser parser("test");
    parser.addArg("t", "tt", "test", callBackD, 0, true);
    parser.addArg("x", "xx", "test", callBackD, 0, true);
    parser.addArg("a", "aa", "test", callBackD, 0, true);
    char *arg[] = {"program","t","xx","aa" };
    ASSERT_TRUE(parser.analyseArgv(4,arg));
}

TEST(argvParser,emptyConfig_testHelpMessage){
    argvParser parser("empty conf");
    ASSERT_EQ("\x1B[1;32mempty conf\nusage:\n\x1B[0m \n"
              ,parser.getHelpMessage());
}

TEST(argvParser,testHelpMessage){
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("a", "aaa", "helpMessage", nullptr, 0, false));
    ASSERT_EQ("\x1B[1;32mempty conf\nusage:\n\t<a> \t <aaa> \t : helpMessage\n\x1B[0m \n"
              ,parser.getHelpMessage());
}
TEST(argvParser,testHelpMessageTwoArgs){
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("a", "aaa", "helpMessage", nullptr, 0, false));
    ASSERT_TRUE(parser.addArg("b", "bbb", "helpMessage2", nullptr, 0, false));
    ASSERT_EQ("\x1B[1;32mempty conf\nusage:\n\t<a> \t <aaa> \t : helpMessage\n\t<b> \t <bbb> \t : helpMessage2\n\x1B[0m \n"
              ,parser.getHelpMessage());
}


TEST(argvParser,argWithCallBackAndNoMachingArg){
    argvParser parser("empty conf");
    char *arg[] = {"program", "-p" ,"61234","-d"};
    parser.addArg("", "", "", nullptr, 0, false);
    ASSERT_FALSE(parser.analyseArgv(4,arg));

}

TEST(argvParser,argWithCallBackAndNoMachingArg2){
    argvParser parser("empty conf");
    char *arg[] = {"program", "-p" ,"61234","-d"};
    parser.addArg("-x", "-x", "xxx", nullptr, 0, false);
    ASSERT_FALSE(parser.analyseArgv(4,arg));

}
TEST(argvParser,argWithCallBackAndNoMachingArg3){
    char *arg[] = {"program", "-port" ,"61234","-dd","blub"};
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-p", "-port", "add Port", callBackPortTestMeth, 0, false));
    ASSERT_TRUE(parser.addArg("-d", "-dd", "add d", callBackD, 0, false));
    port_ = "";
    d = false;
    ASSERT_FALSE(parser.analyseArgv(5,arg));
    ASSERT_EQ(port_,"61234");
    ASSERT_TRUE(d);

}
TEST(argvParser,argWithCallBackShOrt){
    char *arg[] = {"program", "-p" ,"61234","-d"};
    argvParser parser("empty conf");
    parser.addArg("-p", "-port", "add Port", callBackPortTestMeth, 0, false);
    parser.addArg("-d", "-dd", "add d", callBackD, 0, false);
    port_ = "";
    d = false;
    ASSERT_TRUE(parser.analyseArgv(4,arg));
    ASSERT_EQ(port_,"61234");
    ASSERT_TRUE(d);

}
TEST(argvParser,argWithCallBackLong){
    char *arg[] = {"program", "-port" ,"61234","-dd"};
    argvParser parser("empty conf");
    parser.addArg("-p", "-port", "add Port", callBackPortTestMeth, 0, false);
    parser.addArg("-d", "-dd", "add d", callBackD, 0, false);
    ASSERT_TRUE(parser.analyseArgv(4,arg));
    ASSERT_EQ(port_,"61234");
    ASSERT_TRUE(d);

}

TEST(argvParser,NoArgumentsPassed){
    char *arg[] = {"program", "a" };
    argvParser parser("empty conf");
    parser.addArg("-p", "-port", "add Port", callBackPortTestMeth, 0, false);
    parser.addArg("-d", "-dd", "add d", callBackD, 0, false);
    ASSERT_TRUE(parser.analyseArgv(1,arg));
    ASSERT_EQ(port_,"61234");
    ASSERT_TRUE(d);

}

TEST(argvParser,ArgumentIsMoreThanDefined){
    char *arg[] = {"program", "abcd" };
    argvParser parser("test");
    parser.addArg("abc", "abc", "add something", nullptr, 0, false);
    ASSERT_FALSE(parser.analyseArgv(2,arg));

}


TEST(argvParser,addShortArgTwice){
    char *arg[] = {"program", "a" };
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-p", "-abc", "add Port", callBackPortTestMeth, 0, false));
    ASSERT_FALSE(parser.addArg("-p", "-dd", "add d", callBackD, 0, false));


}

TEST(argvParser,addLongArgTwice1){
    char *arg[] = {"program", "a" };
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-xx", "-abc", "add Port", callBackPortTestMeth, 0, false));
    ASSERT_FALSE(parser.addArg("-p", "-abc", "add d", callBackD, 0, false));


}
TEST(argvParser,addLongArgumentEqToExistingShortArg){
    char *arg[] = {"program", "-a" };
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-abc", "-xx", "add Port", callBackPortTestMeth, 0, false));
    ASSERT_FALSE(parser.addArg("-p", "-abc", "add d", callBackD, 0, false));


}

TEST(argvParser,requiredArgsNotSearchedJet){
    char *arg[] = {"program", "-a" };
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-a", "-aa", "aa", nullptr, 0, true));
    ASSERT_FALSE(parser.foundAllRequierdArgs());

}
TEST(argvParser,requiredArgsNotSearchedButNotAllFound){
    char *arg[] = {"program", "-a" };
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-a", "-aa", "aa", callBackD, 0, true));
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD, 0, true));
    ASSERT_FALSE(parser.analyseArgv(2,arg));
    ASSERT_FALSE(parser.foundAllRequierdArgs());

}

TEST(argvParser,requiredArgsNotSearchedButNotAllFoundOther){
    char *arg[] = {"program", "-b" };
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-a", "-aa", "aa", callBackD, 0, true));
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD, 0, false));
    ASSERT_FALSE(parser.analyseArgv(2,arg));
    ASSERT_FALSE(parser.foundAllRequierdArgs());

}
TEST(argvParser,requiredArgsNotSearchedButNotAllFoundCorrect){
    char *arg[] = {"program", "-a" };
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-a", "-aa", "aa", callBackD, 0, true));
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD, 0, false));
    ASSERT_TRUE(parser.analyseArgv(2,arg));
    ASSERT_TRUE(parser.foundAllRequierdArgs());

}


TEST(readConfigFromFileAndCallBack,test1){
    char *arg[] = {"program", "../tests/configFiles/config6" };
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-a", "-aa", "aa", callBackD, 0, true));
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD, 0, false));
    ASSERT_FALSE(parser.analyseArgv(2,arg));
    ASSERT_FALSE(parser.foundAllRequierdArgs());
}


//123  b

TEST(readConfigFromFileAndCallBack,test2){
    char *arg[] = {"program", "../tests/configFiles/config7" };
    argvParser parser("empty conf");
    port_ = "";
    ASSERT_TRUE(parser.addArg("-p", "-pp", "p", callBackD, 0, true));
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD, 0, true));
    ASSERT_FALSE(parser.analyseArgv(2,arg));
    ASSERT_FALSE(parser.foundAllRequierdArgs());
}
TEST(readConfigFromFileAndCallBack,test3){
    char *arg[] = {"program", "../tests/configFiles/config7" };
    argvParser parser("empty conf");
    port_ = "";
    ASSERT_TRUE(parser.addArg("-p", "-pp", "p", callBackPortTestMeth, 0, true));
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD, 0, false));
    ASSERT_TRUE(parser.analyseArgv(2,arg));
    ASSERT_TRUE(parser.foundAllRequierdArgs());
    ASSERT_EQ(port_,"123  b");
}

TEST(readConfigFromFileAndCallBack,test4){
    char *arg[] = {"program", "-p", "xx", "../tests/configFiles/config7" };
    argvParser parser("empty conf");
    port_ = "";
    ASSERT_TRUE(parser.addArg("-p", "-pp", "p", callBackPortTestMeth, 0, true));
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD, 0, false));
    ASSERT_TRUE(parser.analyseArgv(4,arg));
    ASSERT_TRUE(parser.foundAllRequierdArgs());
    ASSERT_EQ(port_,"123  b");
}