#include <tchar.h>
#include <gtest\gtest.h>
#include "amt.h"

#include "TestCar.h"
#include "TestCarDepot.h"
#include "TestTask.h"
#include "TestTaskDepot.h"
#include "TestJob.h"
#include "TestJobDepot.h"
#include "TestAMTService.h"

int _tmain(int argc, _TCHAR* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

	return 0;
}

