//
// Created by basto on 10/16/18.
//

#include <gtest/gtest.h>
#include "../src/argvParser.h"
#include "../src/configfileReader.h"

TEST(configReader, readEmptyText){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
}

TEST(configReader, readEmptyTextSpace){
    configfileReader *reader = new configfileReader(" ");
    reader->inputSourceIsString(" ");
    ASSERT_TRUE(!reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
}

TEST(configReader, readEmptyTextSpaceOften){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString(" ");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
}

TEST(configReader, readEmptyTextManySpace){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("    ");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
}

TEST(configReader, readSingeChar){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("a");
    ASSERT_EQ(reader->readUntilNextSeparator(),"a");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
}
TEST(configReader, readSingeWord){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
}

TEST(configReader, readSingeWordSpace){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc ");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
}

TEST(configReader, readS2Word2Space){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc def ");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"def");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
}
TEST(configReader, readSomeCharsSomeSpace){
    configfileReader *reader = new configfileReader("");
    ASSERT_FALSE(reader->faildToOpen());
    reader->inputSourceIsString("abc   def ");
    ASSERT_FALSE(reader->faildToOpen());
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"def");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_FALSE(reader->faildToOpen());
}


TEST(configReader, readSomeWordSomeSpace){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc   def ");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"def");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
}

TEST(configReader, readSomeWordSomeSpace2){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc   def hallo welt  i am a reader");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"def");
    ASSERT_EQ(reader->readUntilNextSeparator(),"hallo");
    ASSERT_EQ(reader->readUntilNextSeparator(),"welt");
    ASSERT_EQ(reader->readUntilNextSeparator(),"i");
    ASSERT_EQ(reader->readUntilNextSeparator(),"am");
    ASSERT_EQ(reader->readUntilNextSeparator(),"a");
    ASSERT_EQ(reader->readUntilNextSeparator(),"reader");

    ASSERT_EQ(reader->readUntilNextSeparator(),"");
}

TEST(configReader, readSeperator){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("\"abc   def\"");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc   def");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");

}

TEST(configReader, justSeperator){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("\"\"");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_TRUE(reader->isEndofFile());

}

TEST(configReader, justSeperatorWithbefor){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("abc \"\"");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_TRUE(!reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_TRUE(reader->isEndofFile());

}

TEST(configReader, seperatorWithSomeText){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("\"abc \" def  hallo \"a b c\" ");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc ");
    ASSERT_EQ(reader->readUntilNextSeparator(),"def");
    ASSERT_EQ(reader->readUntilNextSeparator(),"hallo");
    ASSERT_EQ(reader->readUntilNextSeparator(),"a b c");

}
TEST(configReader, seperatorWithSomeText2){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString("\"abc\"def  hallo \"a b c\" ");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"def");
    ASSERT_EQ(reader->readUntilNextSeparator(),"hallo");
    ASSERT_EQ(reader->readUntilNextSeparator(),"a b c");

}

TEST(configReader, justSeperatorWithAfter){
    configfileReader *reader = new configfileReader("");
    reader->inputSourceIsString(" \"\" abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeparator(),"");

}



TEST(confReaderFile,file1){
    configfileReader *reader = new configfileReader("../tests/config1");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"def");
    ASSERT_EQ(reader->readUntilNextSeparator(),"xxx");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeparator(),"");

}

TEST(confReaderFile,file2){
    configfileReader *reader = new configfileReader("../tests/config2");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"def");
    ASSERT_EQ(reader->readUntilNextSeparator(),"xxx");
    ASSERT_EQ(reader->readUntilNextSeparator(),"hallo");
    ASSERT_EQ(reader->readUntilNextSeparator(),"world");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeparator(),"");

}
TEST(confReaderFile,file3){
    configfileReader *reader = new configfileReader("../tests/config3");
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_EQ(reader->readUntilNextSeparator(),"def");
    ASSERT_EQ(reader->readUntilNextSeparator(),"xxx");
    ASSERT_EQ(reader->readUntilNextSeparator(),"hallo world");
    ASSERT_EQ(reader->readUntilNextSeparator(),"yyy");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeparator(),"");

}

TEST(confReaderFile,file4){
    configfileReader *reader = new configfileReader("../tests/config4");
    ASSERT_FALSE(reader->faildToOpen());
    ASSERT_EQ(reader->readUntilNextSeparator(),"abc");
    ASSERT_FALSE(reader->faildToOpen());
    ASSERT_EQ(reader->readUntilNextSeparator(),"def");
    ASSERT_EQ(reader->readUntilNextSeparator(),"xxx");
    ASSERT_EQ(reader->readUntilNextSeparator(),"hallo world");
    ASSERT_EQ(reader->readUntilNextSeparator(),"yyy");
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_FALSE(reader->faildToOpen());

}

TEST(confReaderFile,noSuchFile){
    configfileReader *reader = new configfileReader("../tests/configxx");
    ASSERT_FALSE(reader->faildToOpen());
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_FALSE(reader->faildToOpen());
    ASSERT_EQ(reader->readUntilNextSeparator(),"");
    ASSERT_FALSE(reader->faildToOpen());
    ASSERT_TRUE(reader->isEndofFile());
    ASSERT_EQ(reader->readUntilNextSeparator(),"");

}