//
// Created by basto on 10/16/18.
//

#include "gtest/gtest.h"
#include "../src/argvParser.h"
#include "../src/configfileReader.h"

TEST(configReader, readEmptyText){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
}

TEST(configReader, readEmptyTextSpace){
    configfileReader *reader = new configfileReader(" ");
    reader->inputSourceIsString(" ");
    ASSERT_TRUE(!reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
}

TEST(configReader, readEmptyTextSpaceOften){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString(" ");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
}

TEST(configReader, readEmptyTextManySpace){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("    ");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
}

TEST(configReader, readSingeChar){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("a");
    ASSERT_EQ(reader->readUntilNextSeperator(),"a");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
}
TEST(configReader, readSingeWord){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
}

TEST(configReader, readSingeWordSpace){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc ");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
}

TEST(configReader, readS2Word2Space){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc def ");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"def");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
}
TEST(configReader, readSomeCharsSomeSpace){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc   def ");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"def");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
}


TEST(configReader, readSomeWordSomeSpace){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc   def ");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"def");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
}

TEST(configReader, readSomeWordSomeSpace2){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc   def hallo welt  i am a reader");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"def");
    ASSERT_EQ(reader->readUntilNextSeperator(),"hallo");
    ASSERT_EQ(reader->readUntilNextSeperator(),"welt");
    ASSERT_EQ(reader->readUntilNextSeperator(),"i");
    ASSERT_EQ(reader->readUntilNextSeperator(),"am");
    ASSERT_EQ(reader->readUntilNextSeperator(),"a");
    ASSERT_EQ(reader->readUntilNextSeperator(),"reader");

    ASSERT_EQ(reader->readUntilNextSeperator(),"");
}

TEST(configReader, readSeperator){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("\"abc   def\"");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc   def");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");

}

TEST(configReader, justSeperator){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("\"\"");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
    ASSERT_TRUE(reader->isEndofFile());

}

TEST(configReader, justSeperatorWithbefor){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc \"\"");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_TRUE(!reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeperator(),"");
    ASSERT_TRUE(reader->isEndofFile());

}

TEST(configReader, seperatorWithSomeText){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("\"abc \" def  hallo \"a b c\" ");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc ");
    ASSERT_EQ(reader->readUntilNextSeperator(),"def");
    ASSERT_EQ(reader->readUntilNextSeperator(),"hallo");
    ASSERT_EQ(reader->readUntilNextSeperator(),"a b c");

}
TEST(configReader, seperatorWithSomeText2){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("\"abc\"def  hallo \"a b c\" ");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"def");
    ASSERT_EQ(reader->readUntilNextSeperator(),"hallo");
    ASSERT_EQ(reader->readUntilNextSeperator(),"a b c");

}

TEST(configReader, justSeperatorWithAfter){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString(" \"\" abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeperator(),"");

}



TEST(confReaderFile,file1){
    configfileReader *reader = new configfileReader("../tests/config1");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"def");
    ASSERT_EQ(reader->readUntilNextSeperator(),"xxx");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeperator(),"");

}

TEST(confReaderFile,file2){
    configfileReader *reader = new configfileReader("../tests/config2");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"def");
    ASSERT_EQ(reader->readUntilNextSeperator(),"xxx");
    ASSERT_EQ(reader->readUntilNextSeperator(),"hallo");
    ASSERT_EQ(reader->readUntilNextSeperator(),"world");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeperator(),"");

}
TEST(confReaderFile,file3){
    configfileReader *reader = new configfileReader("../tests/config3");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"def");
    ASSERT_EQ(reader->readUntilNextSeperator(),"xxx");
    ASSERT_EQ(reader->readUntilNextSeperator(),"hallo world");
    ASSERT_EQ(reader->readUntilNextSeperator(),"yyy");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeperator(),"");

}

TEST(confReaderFile,file4){
    configfileReader *reader = new configfileReader("../tests/config4");
    ASSERT_EQ(reader->readUntilNextSeperator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeperator(),"def");
    ASSERT_EQ(reader->readUntilNextSeperator(),"xxx");
    ASSERT_EQ(reader->readUntilNextSeperator(),"hallo world");
    ASSERT_EQ(reader->readUntilNextSeperator(),"yyy");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeperator(),"");

}