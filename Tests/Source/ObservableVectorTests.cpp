/*
    Copyright (c) 2018 Xavier Leclercq

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include "ObservableVectorTests.h"

using namespace Ishiko::TestFramework;

void ObservableVectorTests::AddTests(TestHarness& theTestHarness)
{
    TestSequence& vectorTestSequence = theTestHarness.appendTestSequence("ObservableVector tests");

    new HeapAllocationErrorsTest("Creation test 1", CreationTest1, vectorTestSequence);

    new HeapAllocationErrorsTest("pushBack test 1", PushBackTest1, vectorTestSequence);
    new HeapAllocationErrorsTest("pushBack test 2", PushBackTest2, vectorTestSequence);
    new HeapAllocationErrorsTest("pushBack test 3", PushBackTest3, vectorTestSequence);
}

TestResult::EOutcome ObservableVectorTests::CreationTest1()
{
    Ishiko::Collections::ObservableVector<int> vector;
    if (vector.size() == 0)
    {
        return TestResult::ePassed;
    }
    else
    {
        return TestResult::eFailed;
    }
}

TestResult::EOutcome ObservableVectorTests::PushBackTest1()
{
    Ishiko::Collections::ObservableVector<int> vector;
    vector.pushBack(123);
    if ((vector.size() == 1) && (vector[0] == 123))
    {
        return TestResult::ePassed;
    }
    else
    {
        return TestResult::eFailed;
    }
}

TestResult::EOutcome ObservableVectorTests::PushBackTest2()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer);

    vector.pushBack(123);
    if ((vector.size() == 1) && (vector[0] == 123))
    {
        if ((observer->m_additions.size() == 1) &&
            (observer->m_additions[0] == std::pair<size_t, int>(0, 123)))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome ObservableVectorTests::PushBackTest3()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer);

    vector.pushBack(123);
    vector.pushBack(456);
    vector.pushBack(789);
    if ((vector.size() == 3) && (vector[0] == 123) && (vector[1] == 456) && (vector[2] == 789))
    {
        if ((observer->m_additions.size() == 3) &&
            (observer->m_additions[0] == std::pair<size_t, int>(0, 123)) &&
            (observer->m_additions[1] == std::pair<size_t, int>(1, 456)) && 
            (observer->m_additions[2] == std::pair<size_t, int>(2, 789)))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

void IntVectorObserver::onElementAdded(size_t pos, const int& value)
{
    m_additions.push_back(std::pair<size_t, int>(pos, value));
}
