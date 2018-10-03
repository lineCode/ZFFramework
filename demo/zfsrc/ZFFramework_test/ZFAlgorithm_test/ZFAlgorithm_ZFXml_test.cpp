/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFXml_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFXml_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFXml");

        const zfchar *src =
            "<?xml type=\"declaration\"?>"
            "<!DOCTYPE TYPE \"DOCTYPE\">"
            "<?pi ProcessingInstruction?>"
            "<element0 attribute0=\"attribute value0\">"
            "<element1 attribute0=\"attribute value0\" />"
            "<element2 attribute0=\"attribute value0\" attribute1=\"attribute value1\""
            " attribute2=\"attribute value2\" attribute3=\"attribute value3\" />"
            "<element3>text0</element3>"
            "<element4><![CDATA[cdata text]]></element4>"
            "<element5>text with escaped chars: &lt; &gt; &amp; &apos; &quot;</element5>"
            "</element0>"
            ;

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFXml parse from string:");
        ZFXmlItem doc = ZFXmlItemFromString(src);

        this->testCaseOutput("ZFXml parse from string, result:");
        zfLogTrimT() << ZFXmlItemToString(doc);

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFXml add element");
        ZFXmlItem element(ZFXmlType::e_XmlElement);
        element.xmlNameSet("testElement");
        doc.xmlChildAdd(element);
        this->testCaseOutput("ZFXml add element, result:");
        zfLogTrimT() << ZFXmlItemToString(doc);

        this->performanceTest(doc);

        this->testCaseStop();
    }
private:
    void performanceTest(ZF_IN const ZFXmlItem &xmlItem)
    {
        zfindex toDataTimes = 10000;
        ZFCoreStatistic::invokeTimeAccurateLogBegin("ZFXmlPerformance_test_toData");
        for(zfindex i = 0; i < toDataTimes; ++i)
        {
            zfstring tmp;
            ZFXmlItemToOutput(ZFOutputForString(tmp), xmlItem);
        }
        ZFCoreStatistic::invokeTimeAccurateLogEnd("ZFXmlPerformance_test_toData");

        zfindex fromDataTimes = toDataTimes;
        zfstring xmlString;
        ZFXmlItemToOutput(ZFOutputForString(xmlString), xmlItem);
        ZFCoreStatistic::invokeTimeAccurateLogBegin("ZFXmlPerformance_test_fromData");
        for(zfindex i = 0; i < fromDataTimes; ++i)
        {
            ZFXmlItemFromInput(ZFInputForBuffer(xmlString.cString()));
        }
        ZFCoreStatistic::invokeTimeAccurateLogEnd("ZFXmlPerformance_test_fromData");
        ZFXmlItem xmlItemNew = ZFXmlItemFromInput(ZFInputForBuffer(xmlString.cString()));

        this->testCaseOutputSeparator();

        ZFTimeValue toDataUsedTime = ZFCoreStatistic::invokeTimeAccurateGetTotalTime("ZFXmlPerformance_test_toData");
        this->testCaseOutput("serialize to data %zi times cost %s seconds",
            toDataTimes,
            ZFTimeValueToStringFriendly(toDataUsedTime).cString());

        ZFTimeValue fromDataUsedTime = ZFCoreStatistic::invokeTimeAccurateGetTotalTime("ZFXmlPerformance_test_fromData");
        this->testCaseOutput("serialize from data %zi times cost %s seconds",
            fromDataTimes,
            ZFTimeValueToStringFriendly(fromDataUsedTime).cString());

        #if 0
            this->testCaseOutputSeparator();
            this->testCaseOutput("content: %s", xmlString.cString());
            this->testCaseOutput("newly: %s", ZFXmlItemToString(xmlItemNew).cString());
        #endif

        ZFCoreStatistic::invokeTimeAccurateRemove("ZFXmlPerformance_test_toData");
        ZFCoreStatistic::invokeTimeAccurateRemove("ZFXmlPerformance_test_fromData");
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFXml_test)

ZF_NAMESPACE_GLOBAL_END

