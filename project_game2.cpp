#include <bangtal.h>
#include <math.h>
#include <stdlib.h>
//장면생성 시작화면, 게임화면, 설명화면
SceneID scene0,scene1,scene2;
//게임화면,주인공,체크표시,벽1,벽2
ObjectID map1, angel1,check,wall1,wall2;
//적6개 생성
ObjectID enemy[6];
//공 5개 생성
ObjectID ball[5];

//버튼 생성
ObjectID startbutton, howbutton, endbutton, restartbutton, startbutton2;

//적들의 x, y좌표 생성
double ex[6],ey[6];
//총알 번호 0~9
int ballnum;
//(p,k)는 현재 주인공의 위치, (a,b)는 각각 x,y방향 벡터값임
double p, k, a, b;
//(p_ball,k_ball)는 현재 공의 위치,(vecball_x,vecball_y)공의 벡터값
double p_ball[5], k_ball[5], vecball_x[5], vecball_y[5];

//공이 벽에 튕긴 횟수를 저장하는 변수
int hit[5];

//클릭한 곳의 x값과 ,y값을 저장하는 변수
double c, d;
//클릭한 곳의 x값과 ,y값을 저장하는 변수
double c_ball, d_ball;

//t = 거리 10씩 이동시 필요한 칸 수
double t;
double t_ball[5];

//타이머1:체크함수 타이머, timermove: 움직임 타이머, timerball: 공의 움직임 타이머, timerenem[6]: 적의 움직임 타이머
TimerID timer1,timermove;
TimerID timerball[5];
TimerID timerenem[6];

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

//움직임 함수
//move(objecID,한번에 이동하는 x좌표,한번에 이동하는 y좌표)
void move(ObjectID object, double x, double y) {


	locateObject(object, scene1, p + x, k + y);
	p += x;
	k += y;
	//움직임함수 실행마다 다시 타이머를 실행시킨다.
	setTimer(timermove, 0.025f);
	startTimer(timermove);
	
	

}

//move_ball
//move_ball(objectID, 타이머, 현재 x좌표, 현재 y좌표, 벡터 x값, 벡터 y값)

/*
void move_ball(ObjectID object,TimerID timer,float x, float y, float vec_x, float vec_y) {



	locateObject(object, scene1, x + vec_x, y + vec_y);
	x += vec_x;
	y += vec_y;
	setTimer(timer, 0.1f);
	startTimer(timer);



}
*/

//마우스 콜백함수
void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	//시작버튼 클릭시 시작
	if (object == startbutton || object == startbutton2) {
		enterScene(scene1);

	}
	else if (object == howbutton) {
		enterScene(scene2);
	}
	else if (object == endbutton) {
		endGame();

	}
	
	//만약 바닥을 찍었다면 체크무늬 0.2초간 생겼다가 사라짐
	
	//바닥에서만 움직일 수 있도록, 벽은 찍어도 체크표시 생성x
	if (y <= 350) {
		if (object == map1) {
			setTimer(timer1, 0.5f);

			c = x;
			d = y;
			//체크함수 타이머 작동
			startTimer(timer1);

			// 0.1초에 한번씩 주인공이 움직이는 타이머 작동
			setTimer(timermove, 0.1f);
			startTimer(timermove);
			
			//0.1초에 한번씩 적이 움직이는 타이머 작동
			for (int i = 0; i <= 5; i++) {
			setTimer(timerenem[i], 0.1f);
			startTimer(timerenem[i]);
			}
			
			//t 값은 주인공이 목표까지 가는데 몇칸인지 알려줌
			t = sqrt((x - p) * (x - p) + (y - k) * (y - k)) / 10;
			//a=주인공의 x축 벡터값
			a = (x - p) / t;
			//b=주인공의 y축 벡터값
			b = (y - k) / t;
			//체크무늬가 찍은곳에 생긴다.
			locateObject(check, scene1, x - 20, y - 20);
			showObject(check);


		}
	}

	//만약 벽을 클릭했다면
	//클릭한 벽의 좌표를 저장 하고 사용된 공의 갯수를 추가함.
	if (object == wall2) {

		//0.1초에 한번씩 움직이는 타이머 작동
		for (int i = 0; i <= 5; i++) {
			setTimer(timerenem[i], 0.1f);
			startTimer(timerenem[i]);
		}

		for (int i = 0; i <= 4; i++) {
			if (i == ballnum) {
				//먼저 공을 주인공 근처에 생성
				p_ball[i] = p + 93;
				k_ball[i] = k;
				
				//			
				locateObject(ball[i], scene1, p_ball[i], k_ball[i]);
				showObject(ball[i]);
				c_ball = x;
				d_ball = y + 325.0;



				//t[i]값은 공이 목표까지 가는데 몇칸인지 알려줌
				t_ball[i] = sqrt((x - p_ball[i]) * (x - p_ball[i]) + ((double)y + 325 - k_ball[i]) * ((double)y+  325 - k_ball[i])) / 10;

				vecball_x[i] = (c_ball - p_ball[i]) / t_ball[i];
				vecball_y[i] = (d_ball - k_ball[i]) / t_ball[i];

				//0.1초에 한번씩 움직이는 타이머 작동
				setTimer(timerball[i], 0.01f);
				
				startTimer(timerball[i]);
			}
		
		}
		ballnum += 1;
	}
	if (object == wall1) {
		//0.1초에 한번씩 움직이는 타이머 작동
		for (int i = 0; i <= 5; i++) {
			setTimer(timerenem[i], 0.1f);
			startTimer(timerenem[i]);
		}

		for (int i = 0; i <= 4; i++) {
			if (i == ballnum) {
				//먼저 공을 주인공 근처에 생성
				p_ball[i] = p + 93;
				k_ball[i] = k;

				locateObject(ball[i], scene1, p_ball[i], k_ball[i]);
				showObject(ball[i]);
				c_ball = x;
				d_ball = y;



				//t[i]값은 공이 목표까지 가는데 몇칸인지 알려줌
				t_ball[i] = sqrt((x - p_ball[i]) * (x - p_ball[i]) + (d_ball - k_ball[i]) * (d_ball - k_ball[i])) / 10;

				vecball_x[i] = (c_ball - p_ball[i]) / t_ball[i];
				vecball_y[i] = (d_ball - k_ball[i]) / t_ball[i];

				//0.1초에 한번씩 움직이는 타이머 작동
				setTimer(timerball[i], 0.01f);

				startTimer(timerball[i]);
			}

		}
		ballnum += 1;

	}
}




//타이머 콜백함수
void timerCallback(TimerID timer) {
	//공이 적과 충돌시 소멸

	//ex[6],ey[6]; 적의 좌표
	// p_ball[10], k_ball[10]: 공의 좌표
	//x좌표: 왼쪽선: 적의 왼쪽 y좌표~y+125,  오른쪽선: 적의 오른쪽 y좌표~ y+125,    //y좌표: 위쪽선: 적의 머리 쪽 x+48~ x+123    아래쪽선: 적의 발쪽 x+48~x+125

	//2.오른쪽선

	//적이동 
	int n = 0;
	for (int i = 0; i <= 5; i++) {
		if (ex[i] > 1280) {
			n = n + 1;

		}

		if (n == 5) {
			showMessage("승리");

		}
	}


	for (int i = 0; i <= 5; i++) {

		if (ex[i] < 0) {
			enterScene(scene0);

		}
		
		else if (timer == timerenem[i]) {
				locateObject(enemy[i], scene1, ex[i] - 10, 10 + ey[i]);
				ex[i] -= 10;
				ey[i] += 0;
				setTimer(timerenem[i], 0.01f);
				startTimer(timerenem[i]);
			}
		}
	
	//만일 타임아웃되어 전달받은 파라미터 값이 타이머1이라면 체크를 사라지게한다.
	if (timer == timer1) {
		hideObject(check);
	}


	//만일 움직임 타이머가 0.1초가 끝난다면 타이머를 다시 시작하고 현재 위치에서 체크무늬를 찍은곳까지로 한번에 (x-p)/t씩 (y-k)/t 씩움직임.
	if (timer == timermove) {

		move(angel1, a, b);
		//주인공이 x 체크범위내에 들어오면 x방향 이동 멈춤
		if (c - 50 <= p && p <= c - 20) {
			stop(1, 0);

		}
		//주인공이 y 체크범위내에 들어오면 y방향 이동 멈춤
		if (d - 10 <= k && k <= d + 10) {
			stop(0, 1);

		}
		if ((d - 10 <= k && k <= d + 10) && (c - 50 <= p && p <= c - 20)) {

			stop(1, 1);
		}
	}

	//만일 공의 타이머가 끝난다면 다시 시작하고 좌표까지 x,y벡터값씩 이동
	for (int i = 0; i <= 4; i++) {
		
		if (timer == timerball[i]) {
			
			//작동하지않음.. 왜일까..
			//move_ball(ball[i], timerball[i], p_ball[i], k_ball[i], vecball_x, vecball_y);
			//벽에 충돌시 튕긴다.
			if (k_ball[i] > 305 || k_ball[i] <= 0) {
		
				vecball_y[i] = -vecball_y[i];
				hit[i] += 1;
			}

			

			locateObject(ball[i], scene1, p_ball[i] + vecball_x[i], k_ball[i] + vecball_y[i]);
			p_ball[i] += vecball_x[i];
			k_ball[i] += vecball_y[i];
			setTimer(timerball[i], 0.0001f);
			startTimer(timerball[i]);
			
		}
		//벽에 세번 충돌시 사라짐
		if (hit[i] == 3) {
			hideObject(ball[i]);
			locateObject(ball[i], scene0, -200, -200);
			stopTimer(timerball[i]);

		}
		

		//적과 충돌시 공 소멸
		for (int i = 0; i <= 4; i++) {

			for (int k = 0; k <= 5; k++) {

				// 크기 두배였을때는 if ((p_ball[i] + 40 >= ex[k] + 48 && p_ball[i] <= ex[k] + 123) && (k_ball[i] + 40 > ey[k] && k_ball[i] <= ey[k] + 110))
				if ((p_ball[i] + 20 >= ex[k] + 24 && p_ball[i] <= ex[k] + 62) && (k_ball[i] + 40 > ey[k] && k_ball[i] <= ey[k] + 60)) {
					//hideObject(ball[i]);
					locateObject(ball[i], scene0, 1200, 900);
					stopTimer(timerball[i]);
					hideObject(enemy[k]);
					locateObject(enemy[k], scene0, 1900, 100);
					stopTimer(timerenem[k]);
				}
			}

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
	
	enemy[3] = createObject("images/enemy1.png");
	locateObject(enemy[3], scene1, 1300, 23);
	showObject(enemy[3]);
	ex[3] = 1300;
	ey[3] = 223;

	enemy[4] = createObject("images/enemy2.png");
	locateObject(enemy[4], scene1, 1300, 23);
	showObject(enemy[4]);
	ex[4] = 1300;
	ey[4] = 23;

	enemy[5] = createObject("images/enemy3.png");
	locateObject(enemy[5], scene1, 1300, 23);
	showObject(enemy[5]);
	ex[5] = 1300;
	ey[5] = 123;
	
	//이게 변경점
	//void scaleObject(ObjectID object, ObjectScale scale)
	//물체(object)의 크기(scale)를 조정한다.
	for (int k = 0; k <= 5; k++) {


		scaleObject(enemy[k], 0.5f);            // 200%
	}

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
	for (int i = 0; i <= 9; i++) {
		timerball[i] = createTimer(3.14f);
		
	}
	for (int i = 0; i <= 5; i++) {



		timerenem[i] = createTimer(3.14f);

	}

	//장면생성
	scene1 = createScene("shooting a wall", "images/backgrou2nd1.png");
	//스타트 화면 생성
	scene0 = createScene("shooting a wall", "images/start.jpg");
	//설명화면 생성
	scene2 = createScene("shooting a wall", "images/howscreen.jpg");
	//장면 전체에 map이라는 오브젝트 씌움
	map1 = createObject("images/background2.png");
	locateObject(map1, scene1, 0, 0);
	showObject(map1);

	p = 20, k = 30;
	
	//버튼 생성
	startbutton = createObject("images/startbutton.png");
	startbutton2 = createObject("images/startbutton.png");
	howbutton = createObject("images/howbutton.png");
	restartbutton = createObject("images/restartbutton.png");
	endbutton = createObject("images/endbutton.png");

	//scene0(시작화면에 배치)
	locateObject(startbutton, scene0, 540, 284);
	showObject(startbutton);
	locateObject(howbutton, scene0, 540, 196);
	showObject(howbutton);
	locateObject(endbutton, scene0, 540, 100);
	showObject(endbutton);
	
	//scene2(설명화면에 배치)
	locateObject(startbutton2, scene2, 540, 15);
	showObject(startbutton2);
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

	//공생성
	for (int i = 0; i <= 4; i++)
	{
		ball[i] = createObject("images/ball.png");
		

	}
	
	
	for (int i = 0; i <= 4; i++) {
		hit[0] = 0;

	}
	startGame(scene0);
	
}
