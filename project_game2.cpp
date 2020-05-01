#include <bangtal.h>
#include <math.h>
#include <stdlib.h>
//장면생성
SceneID scene1;

ObjectID map1, angel1,check,wall1,wall2;
//적생성
ObjectID enemy[3];

//적들의 x, y좌표 생성
float ex[3],ey[3];

//(p,k)는 현재 주인공의 위치
//a,b는 콜백 받은 순간부터 정해지는 상수값임.
float p, k, a, b;

//마우스 콜백 x,y저장하는 변수
float c, d;

//프레임수
float t;

//타이머1,타이머무브
TimerID timer1,timermove;


//x를 멈출껀지 y를 멈출껀지 고름. 정지는 1, 지속 =0,  (1,1):둘다 정지, (1,0): x만 정지, (0,1): y만 정지
void stop(int x,int y) {
	if (x == 1 && y != 1) {
		a = 0;
	}
	else if (x != 1 && y == 1) {
		b = 0;

	}
	else if (x == 1 && y == 1) {
locateObject(angel1, scene1, p, k);
	stopTimer(timermove);

	}
	
}

//무브 함수 움직임
//move(objecID,한번에 이동하는 x좌표,한번에 이동하는 y좌표)
void move(ObjectID object, float x, float y) {


	
	locateObject(object, scene1, p + x, k + y);
	p += x;
	k += y;
	setTimer(timermove, 0.025f);
	startTimer(timermove);
	
	

}

//마우스 콜백함수
void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	//만약 바닥을 찍었다면 체크무늬 0.2초간 생겼다가 사라짐
	setTimer(timer1, 0.2f);
	//바닥에서만 움직일 수 있도록, 벽은 찍어도 체크표시 생성x
	if (y <= 350) {
		if (object == map1) {
			c = x;
			d = y;
			//체크함수 타이머 작동
			startTimer(timer1);

			// 0.1초에 한번씩 움직이는 타이머 작동
			setTimer(timermove, 0.1f);
			startTimer(timermove);

			//t 값은 주인공이 목표까지 가는데 몇칸인지 알려줌
			//a,b는 마우스 콜백 받은 순간부터 정해지는 상수값임.
			t = sqrt((x - p) * (x - p) + (y - k) * (y - k)) / 10;
			a = (x - p) / t;
			b = (y - k) / t;
			//체크무늬가 찍은곳에 생긴다.
			locateObject(check, scene1, x - 20, y - 20);
			showObject(check);


		}
	}
	
}




//타이머 콜백함수
void timerCallback(TimerID timer) {
	


	for (int i = 0; i <= 2; i++) {
		locateObject(enemy[i], scene1, ex[i]-5, 10 + ey[i]);
		ex[i] -= 5;
		ey[i] += 0;
		setTimer(timermove, 0.025f);
		startTimer(timermove);
	}

	//만일 타임아웃되어 전달받은 파라미터 값이 타이머1이라면 체크를 사라지게한다.
	if (timer == timer1) {
		hideObject(check);
	}


	//만일 움직임 타이머가 0.1초가 끝난다면 타이머를 다시 시작하고 현재 위치에서 체크무늬를 찍은곳까지로 한번에 (x-p)/t씩 (y-k)/t 씩움직임.
	if (timer == timermove) {
		
		move(angel1,a,b);
		//주인공이 x 체크범위내에 들어오면 x방향 이동 멈춤
		if (c - 50 <= p && p <= c -20) {
			stop(1,0);

		}
		//주인공이 y 체크범위내에 들어오면 y방향 이동 멈춤
		if (d - 10 <= k && k <= d + 10) {
			stop(0, 1);

		}
		if ((d - 10 <= k && k <= d + 10) && (c - 50 <= p && p <= c - 20)) {

			stop(1, 1);
		}
	}
}

//악당 생성함수
void createenemy() {
	enemy[0] = createObject("images/enemy1.png");
	locateObject(enemy[0], scene1, 1100, 223);
	showObject(enemy[0]);
	ex[0] = 1100;
	ey[0] = 223;


	enemy[1] = createObject("images/enemy2.png");
	locateObject(enemy[1], scene1, 1100, 123);
	showObject(enemy[1]);
	ex[1] = 1100;
	ey[1] = 123;


	enemy[2] = createObject("images/enemy3.png");
	locateObject(enemy[2], scene1, 1100, 23);
	showObject(enemy[2]);
	ex[2] = 1100;
	ey[2] = 23;

	//이게 변경점
}




int main()
{
	//마우스 콜백 호출
	setMouseCallback(mouseCallback);
	//타이머 콜백함수 호출
	setTimerCallback(timerCallback);
	//타이머 선언
	timer1 = createTimer(3.14f);
	timermove = createTimer(3.14f);

	//장면생성
	scene1 = createScene("벽팅구기", "images/backgrou2nd1.png");
	//장면 전체에 map이라는 오브젝트 씌움
	map1 = createObject("images/background2.png");
	locateObject(map1, scene1, 0, 0);
	showObject(map1);

	p = 20, k = 30;
	

	//체크 무늬 생성
	check = createObject("images/check.png");
	//벽생성
	wall1 = createObject("images/wall.png");
	locateObject(wall1, scene1, 0, 0);
	showObject(wall1);
	wall2 = createObject("images/wall.png");
	locateObject(wall2, scene1, 0, 325);
	showObject(wall2);
	
	//주인공 생성
	angel1 = createObject("images/angel1.png");
	locateObject(angel1, scene1, p, k);
	showObject(angel1);
	
	//악당 생성
	createenemy();
	

	startGame(scene1);

}
