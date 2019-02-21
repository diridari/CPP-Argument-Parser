//
// Created by basti on 20.01.2019.
//

#include <gtest/gtest.h>
#include <argvParser.h>

bool lambda = false;

TEST(LambdaCallBack, simpleCallBackWithoutArgumentsAndEmptyLambda) {
    argvParser parser("test");
    lambda = false;
    parser.addArg("a", "aa", "test", []() {});

    char *arg[] = {"program", NULL};


    ASSERT_TRUE(parser.analyseArgv(1, arg));
    ASSERT_FALSE(lambda);
}

TEST(LambdaCallBack, simpleCallBackWithArgumentsAndEmptyLambda) {
    argvParser parser("test");
    lambda = false;
    parser.addArg("a", "aa", "test", []() {});

    char *arg[] = {"program", "a"};


    ASSERT_TRUE(parser.analyseArgv(2, arg));
    ASSERT_FALSE(lambda);
}

TEST(LambdaCallBack, simpleCallBackWithOutArguments) {
    argvParser parser("test");
    lambda = false;
    parser.addArg("a", "aa", "test", []() { lambda = true; });

    char *arg[] = {"program"};


    ASSERT_TRUE(parser.analyseArgv(1, arg));
    ASSERT_FALSE(lambda);
}


TEST(LambdaCallBack, simpleCallBackWithArguments) {
    argvParser parser("test");
    lambda = false;
    parser.addArg("a", "aa", "test", []() { lambda = true; });

    char *arg[] = {"program", "a"};


    ASSERT_TRUE(parser.analyseArgv(2, arg));
    ASSERT_TRUE(lambda);
}

TEST(LambdaCallBack, CallBackWithArguments) {
    argvParser parser("test");
    lambda = false;
    function<int(int, char **)> callBack = [](int i, char **buff) {
        lambda = true;
        return i;
    };
    parser.addArg("a", "aa", "test", callBack);

    char *arg[] = {"program", "a"};


    ASSERT_TRUE(parser.analyseArgv(2, arg));
    ASSERT_TRUE(lambda);
}

TEST(LambdaCallBack, CallBackWithOutArguments) {
    argvParser parser("test");
    lambda = false;
    function<int(int, char **)> callBack = [](int i, char **buff) {
        lambda = true;
        return i;
    };
    parser.addArg("a", "aa", "test", callBack);

    char *arg[] = {"program", "a"};

    ASSERT_TRUE(parser.analyseArgv(1, arg));
    ASSERT_FALSE(lambda);
}

char **l1;

TEST(LambdaCallBack, CallBackWithOutArgumentsAndParam) {
    argvParser parser("test");
    lambda = false;
    function<int(int, char **)> callBack = [](int i, char **buff) {
        lambda = true;
        i++;
        l1 = &buff[i];
        return i;
    };
    parser.addArg("a", "aa", "test", callBack);

    char *arg[] = {"program", "a", "xx"};

    ASSERT_TRUE(parser.analyseArgv(2, arg));
    ASSERT_TRUE(lambda);
    ASSERT_EQ(*l1, "xx");
}

