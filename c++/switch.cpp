namespace {
int f1()
{
	int a = 0;
	switch(a) {
	case 1:
		break;
	//case 1:
	//	break;
	default:
		break;
	}

/*
switch.cpp:7:7: error: duplicate case value '1'
        case 1:
             ^
switch.cpp:5:7: note: previous case defined here
        case 1:
             ^
*/
}
}

void testSwitch() {
	f1();
}