//
// Created by basto on 2/20/18.
//

#include <gtest/gtest.h>

#include <iostream>


// make all private methods public
#define private public

int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();


    return ret;
}

TEST(Shall_Not_Fail,init){
	ASSERT_EQ(1,1);
}

