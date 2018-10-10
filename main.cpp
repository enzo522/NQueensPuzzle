//
//  main.cpp
//  NQueensQuzzle
//
//  Created by KimSungsoo on 11/12/15.
//  Copyright © 2015 KimSungsoo. All rights reserved.
//

#include <iostream>
#include <stdlib.h> // Visual Studio에서 모든 출력이 끝난 후 잠시 대기하기 위해 stdlib.h포함
#include <time.h> // 행 값을 임의의 값으로 설정하기 위해 time.h 포함
using namespace std;

class Queen // Queen클래스
{
protected:
    int m_row; // 행 값을 나타내는 멤버 정수형 변수
    int m_column; // 열 값을 나타내는 멤버 정수형 변수
    Queen* m_neighbor; // 이웃(바로 왼쪽에 위치한 항목)을 가리키는 멤버 포인터 변수
    
public:
    Queen(); // Queen클래스의 기본 생성자
    Queen(int row, int col, Queen* nei); // Queen클래스의 인자 있는 생성자
    ~Queen(); // Queen클래스의 소멸자
    
    bool canAttack(const Queen& q) const; // 현재 객체가 인자로 넘어온 객체를 공격하는지 확인하는 멤버 함수
    bool findSolution(const int n); // 현재 객체 배열의 해답을 구하는 멤버 함수
    bool Advance(const int n); // 현재 객체의 행 값을 하나 증가시키는 멤버 함수
    void Print() const; // 현재 객체의 좌표를 출력하는 멤버 함수
    void PrintBoard(int* chess_board, const int n); // 현재 객체 배열의 위치를 체스판으로 출력하는 멤버 함수
    int getColumn() const; // 현재 객체의 열 값을 반환하는 멤버 함수
};

Queen::Queen() // Queen클래스의 기본 생성자
{
    m_row = 1; // 좌표를 (1, 1)로 설정
    m_column = 1;
    m_neighbor = NULL; // 이웃을 없음으로 설정
}

Queen::Queen(int row, int col, Queen* nei) // Queen클래스의 인자 있는 생성자
: m_row(row), m_column(col), m_neighbor(nei) // 인자로 넘어온 row, col, nei를 각각 객체의 행 값, 열 값, 이웃으로 설정
{ }

Queen::~Queen() // Queen클래스의 소멸자
{ }

bool Queen::canAttack(const Queen& q) const // 현재 객체가 인자로 넘어온 객체 q를 공격하는지 확인하는 멤버 함수
{
    if( m_row == q.m_row || abs(m_row - q.m_row) == abs(m_column - q.m_column) ) // 두 객체의 행 값이 같거나, 가로 차이와 세로 차이가 같으면(대각선으로 만나면) 공격이 가능하므로
        return true; // 공격 가능하다는 의미를 반환
    else // 공격이 불가능하면
    {
        if( m_neighbor ) // 현재 객체의 이웃이 존재할 때
            return (m_neighbor->canAttack(q)); // 현재 객체의 이웃과 인자로 넘어온 객체 q가 서로 공격이 가능한지 확인
        else // 더 이상 이웃이 존재하지 않으면(객체 배열의 첫번째 항목까지 확인했으면) 더 이상 인자로 넘어온 객체 q를 공격할 수 있는 객체가 존재하지 않으므로
            return false; // 안전하다는 의미를 반환
    }
}

bool Queen::findSolution(const int n) // 현재 객체 배열의 해답을 구하는 멤버 함수
{
    Queen *q(this); // 현재 객체 배열을 q에 임시 저장
    
    if( q[0].m_row >= n ) // 객체 배열 첫번째 항목의 행 값이 n보다 크거나 같아지면(사실상 같아지면 진행)
    {
        q[0].m_row = 1; // 객체 배열 첫번째 항목의 행 값을 1로 설정하고
        findSolution(n); // 현재 객체 배열의 해답을 처음부터 다시 구함
    } // 객체 배열 첫번째 항목이 (1, 1)에서 시작하지 않으므로, 해답이 첫번째 항목 처음 위치보다 위에 있을 경우를 위해 확인
    else // 첫번째 항목의 행 값이 n보다 작을 때
    {
        for( int i = 0; i < (n-1); i++ ) // 객체 배열의 첫번째부터 마지막번째까지 첫번째와 두번째, 두번째와 세번째 이러한 방식으로 비교하면 canAttack함수에서 알아서 이전 항목 모두와 비교함
        {
            if( q[i].canAttack(q[i+1]) ) // i번째와 i+1번째를 비교하여 공격이 가능하면
            {
                while( q[i+1].Advance(n) ) // 일단 i+1번째 항목의 행 값을 하나 증가시키고 반환된 값이 true이면 안으로 false이면 나감
                {
                    if( !q[i].canAttack(q[i+1]) ) // i+1번째 항목의 행 값이 하나 증가된 상태로 i번째 항목과 i+1번째 항목을 다시 비교하여 이전의 모든 객체들이 변경된 i+1번째 항목을 공격 가능한지 확인
                        goto next; // 확인 가능하면 next로 넘어가 다음 항목들을 비교(여기서 break를 사용하여 무한 루프를 빠져나가려 했지만 q[i+1].Advance가 실패할 경우(체스판 밖으로 나가게 되어 최상단으로 올려 처음부터 다시 비교하게 하려면) 어쩔 수 없이 goto문을 사용했습니다.)
                }
                findSolution(n); // i+1번째 항목이 최상단으로 올리고, i번째 항목의 행 값을 하나 증가시킨 뒤의 객체 배열을 가지고 처음부터 다시 모든 항목들을 비교
                
            next:
                continue; // 그 어떤 이전 항목들도 i+1번째 객체를 공격할 수 없으므로 다음 객체들을 비교
            }
        }
    }
    
    return true; // 혹시라도 n이 2나 3인 경우에도 문제없이 구동되게 할 수 있게 되어 boolean임을 활용할 수 있을까 싶어 남겨두었습니다.
}

bool Queen::Advance(const int n) // 현재 객체의 행 값을 하나 증가시키는 멤버 함수
{
    m_row++; // 현재 객체의 행 값을 하나 증가
    
    if( m_row <= n ) // 행 값이 n보다 작거나 같으면
        return true; // 정상적인 진행 성공
    else // n보다 커지면 체스판 밖으로 나가게 되는 것이므로 다시 최상단으로 올리고 이미 배치된 바로 이전의 객체의 위치를 옮겨 다시 확인(사실상 완전히 다시 시작하여 배열 첫번째 항목부터 다시 확인함)
    {
        m_row = 1; // 최상단으로 재정렬
        if( m_neighbor ) // 이웃이 있으면
            m_neighbor->Advance(n); // 해당 이웃의 행 값을 하나 증가시킴
        return false; // findSolution함수에서 while문을 빠져 나오자마자 처음부터 다시 체크하기 위해 false 반환
    } // 여기서 이웃이 없으면(첫번째 항목의 값을 1로 바꿔야하는 경우이면) 해답을 구하지 못했다고 하려 해서 사용자가 n값을 해답을 구할 수 없는 2나 3을 입력해도 프로그램이 진행되다 해답을 구하지 못했다는 문구를 출력하려고 했으나 실패했습니다.
}

void Queen::Print() const // 현재 객체의 좌표를 출력하는 멤버 함수
{
    cout << "(" << m_row << ", " << m_column << ")"; // (행, 열)의 모습으로 출력
}

void Queen::PrintBoard(int* chess_board, const int n) // 현재 객체 배열의 위치를 체스판으로 출력하는 멤버 함수
{
    Queen *q(this); // 현재 객체 배열을 q에 임시 저장
    
    for( int i = 0; i < (n*n); i++ ) // 현재 객체 배열 항목들의 위치를 파악하는 반복문(2차원 동적 배열 생성에 어려움을 겪어 크기가 n^2인 1차원 배열을 사용했습니다.)
    {
        for( int j = 0; j < n; j++ ) // j번째 객체 배열 항목의 행 값과 어떠한 수 i를 n으로 나누고 난 몫에 1을 더한 값이 같고, 열 값과 i를 n으로 나누고 난 나머지에 1을 더한 값이 같은 i값이 체스판에서 해당 좌표의 index
        {
            if( q[j].m_row == (i/n + 1) )
                if( q[j].m_column == (i%n + 1) )
                    chess_board[i] = 1; // 배열 chess_board에서 원하는 위치의 index를 찾았으면 해당 위치의 값을 1로 설정
        }
    }
    
    int new_line = 0; // 화면에 n개의 열을 출력하고 다음 줄로 넘어갈 수 있게 해주는 정수형 변수
    for( int i = 0; i < (n*n); i++ ) // 배열 chess_board 첫번째부터 마지막까지 항목들을 출력하는 반복문
    {
        if( (i/n) > new_line ) // i를 n으로 나누고 난 나머지가 0이면 1행, 1이면 2행임에 착안하여 각 행의 모든 항목을 출력하면 다음 줄로(다음 행으로) 넘어가게 함
        {
            cout << endl;
            new_line++; // k행 출력이 끝났으므로 다음 줄로 넘기고 k+1행을 출력할 동안은 다음 줄로 넘기지 않게 설정
        }
        if( chess_board[i] == 1 ) // 객체 배열의 위치에서는 Queen을 의미하는 Q를 출력
            cout << "Q";
        else // 나머지 공백에서는 +를 출력(-로 할까 ㅁ으로 할까 다른 어떤 것으로 할까 고민을 많이 했지만 +로 결정했습니다.)
            cout << "+";
    }
    
}

int Queen::getColumn() const // 현재 객체의 열 값을 반환하는 멤버 함수
{
    return (m_column); // 현재 객체의 열 값 반환
}

int main()
{
    int n; // 사용자가 원하는 크기의 n값을 저장할 정수형 변수
    int menu; // 사용자가 선택하는 메뉴를 저장할 정수형 변수
    cout << "< N Queens Puzzle >\n"; // 소개 멘트
    
    while( true ) // 사용자가 종료를 원할 때까지 반복되는 무한 루프
    {
        while( true ) // 사용자가 제대로 된 정수를 입력할 때까지 반복되는 무한 루프
        {
            cout << "원하는 크기의 N Queens Puzzle을 입력하세요(N >= 4): "; // 사용자가 원하는 크기의 n값을 입력받음
            cin >> n; // 표준 입력으로부터 사용자가 원하는 크기를 입력받아 해당 변수에 저장
                
            if( !cin ) // 정수가 아닌 문자를 입력했을 때 오류 메시지 출력 후 재입력 유도
            {
                cin.clear();
                while( cin.get() != '\n' );
                cout << "정수가 입력되지 않았습니다. 0보다 큰 정수로 다시 입력해주세요.\n";
            }
            else if( n <= 0 ) // 정수를 입력했지만 0 혹은 음의 정수를 입력했을 때 오류 메시지 출력 후 재입력 유도
                cout << "0이거나 음의 정수를 입력하셨습니다. 0보다 큰 정수로 다시 입력해주세요.\n";
            else if( n == 1 ) // 1 Queen Puzzle은 계산할 필요가 없이 한 칸에 (1, 1)을 배열하면 종료
                cout << "Queen들의 좌표는 { (1, 1) } 입니다.\n";
            else if( n < 4 ) // 2나 3을 입력했을 시 오류 메시지 출력 후 4이상의 숫자를 입력하도록 유도
                cout << "2와 3 Queens Puzzle은 정답이 없습니다. 다른 숫자로 다시 입력해주세요.\n";
            else // 4이상의 정수를 입력했을 때 무한 루프를 빠져 나와 진행
                break;
        }
        
        Queen *neighbor = NULL; // 객체의 이웃을 가리키는 포인터 변수
        Queen *q = new Queen[n]; // 크기가 n인 Queen객체 배열 생성
        
        while( true ) // 사용자가 제대로 된 메뉴를 입력할 때까지 반복되는 무한 루프
        {
            cout << "\n< 메뉴를 선택하세요 >\n" << "1. 원하는 행에서 시작\n" << "2. 임의의 행에서 시작\n"
            << "메뉴 선택: "; // 메뉴 출력
            cin >> menu; // 사용자가 입력한 메뉴를 저장
            
            if( !cin ) // 정수가 아닌 문자를 입력했을 때 오류 메시지 출력 후 재입력 유도
            {
                cin.clear();
                while( cin.get() != '\n' );
                cout << "정수가 입력되지 않았습니다. 1, 2 중 원하는 메뉴를 다시 입력해주세요.\n";
            }
            else if( menu == 1 || menu == 2 ) // 1번 혹은 2번 메뉴 선택 시 무한 루프를 빠져 나와 진행
                break;
            else // 1, 2를 제외한 다른 정수를 입력했을 시 오류 메시지 출력 후 재입력 유도
                cout << "잘못된 정수를 입력하셨습니다. 1, 2 중 원하는 메뉴를 다시 입력해주세요.\n";
        }
        
        
        switch( menu ) // 사용자가 선택한 메뉴에 따라 다른 작업을 수행하는 switch-case문
        {
            case 1: // 원하는 행에서 시작 선택 시
            {
                int r; // 사용자가 원하는 행을 저장할 정수형 변수
                
                while( true ) // 사용자가 제대로 된 정수를 입력할 때까지 반복되는 무한 루프
                {
                    cout << "원하는 행을 입력하세요: "; // 원하는 행 입력 유도
                    cin >> r; // 사용자가 입력한 행을 저장
                    
                    if( !cin ) // 정수가 아닌 문자를 입력했을 때 오류 메시지 출력 후 재입력 유도
                    {
                        cin.clear();
                        while( cin.get() != '\n' );
                        cout << "정수가 입력되지 않았습니다. 1~" << n << "사이의 원하는 행을 다시 입력해주세요.\n";
                    }
                    else if( 0 < r && r <= n ) // 0보다 크고 N보다 작은 정수를 입력했을 시 무한 루프를 빠져 나와 진행
                        break;
                    else // 0이하의 정수나 N보다 큰 정수를 입력했을 시 오류 메시지 출력 후 재입력 유도
                        cout << "0이하거나 " << n << "보다 큰 정수를 입력하셨습니다. 1~" << n << "사이의 원하는 행을 다시 입력해주세요.\n";
                }
                
                srand((unsigned)time(NULL)); // 임의의 값을 생성하기 위해 시드 변경
                q[0] = { Queen(r, 1, neighbor) }; // 객체 배열의 첫번째 항목의 행 값을 사용자가 입력한 값으로, 열 값은 1로, 이웃은 없음으로 설정
                for( int i = 1; i < n; i++ ) // 두번째 항목부터 마지막 항목까지 수행하는 반복문
                {
                    neighbor = &q[i-1]; // 이웃은 바로 이전 항목의 주소값으로 설정
                    q[i] = { Queen((rand() % n) + 1, (i+1), neighbor) }; // 각 항목의 행 값은 1~n사이의 임의의 값으로, 열 값은 2~n까지로, 이웃은 각각의 바로 이전 항목으로 설정
                }
            }
                break; // 초기화가 끝났으므로 switch-case문에서 빠져 나와 진행
                
            case 2: // 임의의 행에서 시작 선택 시
            {
                srand((unsigned)time(NULL)); // 임의의 값을 생성하기 위해 시드 변경
                q[0] = { Queen((rand() % n) + 1, 1, neighbor) }; // 객체 배열의 첫번째 항목의 행 값을 1~n사이의 임의의 값으로, 열 값은 1로, 이웃은 없음으로 설정
                for( int i = 1; i < n; i++ ) // 두번째 항목부터 마지막 항목까지 수행하는 반복문
                {
                    neighbor = &q[i-1]; // 이웃은 바로 이전 항목의 주소값으로 설정
                    q[i] = { Queen((rand() % n) + 1, (i+1), neighbor) }; // 각 항목의 행 값은 1~n사이의 임의의 값으로, 열 값은 2~n까지로, 이웃은 각각의 바로 이전 항목으로 설정
                }
            }
                break; // 초기화가 끝났으므로 switch-case문에서 빠져 나와 진행
                
            default: // 메뉴를 1, 2를 제외한 다른 것으로 입력했을 시
                cout << "메뉴를 잘못 입력하셨습니다. 다시 입력해주세요.\n\n"; // 오류 메시지 출력 후 메뉴 재입력 유도
                continue; // 제대로 된 메뉴 선택까지 계속
        }
        
        cout << "\n[ 초기 좌표 { "; // 소개 멘트
        for( int i = 0; i < n; i++ ) // 객체 배열 첫번째 항목부터 마지막 항목까지 출력
        {
            q[i].Print();
            if( q[i].getColumn() != n ) // i번째 항목의 열 값이 n이 아니면(마지막 항목을 출력하는 것이 아니면)
                cout << ", "; // ,를 출력해 각 항목을 구분
        }
        cout << " } 에서 " << n << " Queens Puzzle을 시작합니다 ]\n\n"; // 소개 멘트
        
        if( !q->findSolution(n) ) // 객체 배열 q의 해답을 구해보고 정답이 없으면
            cout << "정답을 찾지 못했습니다.\n"; // 해답을 구하지 못하면 오류 메시지 출력(첫번째 항목의 시작 위치를 (1, 1)로 설정했으면 n이 2나 3이어도 문제없이 진행되어 정답을 찾지 못했다는 문구를 출력할 수 있었을 텐데 (1, 1)아닌 다른 위치에서 시작하게 했더니 실패했습니다. main함수에서 q[0]의 m_row를 일시저장하여 findSolution과 Advance에 const int인자로 보내서 확인해보려 했는데 해결방안을 찾기 어려웠습니다. 좀 더 고민해보겠습니다.)
        else // 해답을 찾았으면
        {
            cout << "Queen들의 최종 좌표는 { "; // 소개 멘트
            for( int i = 0; i < n; i++ ) // 객체 배열 첫번째 항목부터 마지막 항목까지 좌표 출력
            {
                q[i].Print();
                if( q[i].getColumn() != n ) // i번째 항목의 열 값이 n이 아니면(마지막 항목을 출력하는 것이 아니면)
                    cout << ", "; // ,를 출력해 각 항목을 구분
            }
            cout << " } 입니다.\n\n"; // 소개 멘트
            
            int* chess_board = new int[n*n]{ 0 }; // 체스판을 의미하는 크기가 n^2인 정수형 배열, 모두 0으로 초기화
            q->PrintBoard(chess_board, n); // 객체 배열 q을 체스판 형식으로 출력
            cout << endl << endl; // 줄 바꿈
            delete chess_board; // 체스판 사용이 끝났으므로 메모리 반환
            chess_board = NULL; // 혹시 모를 상황에 대비해 chess_board가 가리키는 것을 없음으로 설정
        }
        
        delete[] q; // 객체 배열의 사용이 끝났으므로 메모리 반환
        q = NULL; // 혹시 모를 상황에 대비해 q가 가리키는 것을 없음으로 설정
        
        while( true ) // 사용자에게 계속할 지 여부를 물어 Y혹은 N을 입력할 때까지 반복되는 무한 루프
        {
            char cont; // 사용자의 의중을 저장할 문자형 변수
            
            cout << "계속 하시겠습니까? (Y/N): "; // 계속 할지 여부 확인
            cin >> cont; // 사용자가 입력한 문자를 저장
            
            if( cont == 'Y' || cont == 'y' )
            {
                cout << "계속합니다.\n\n";
                break;
            } // 문자 중 N 혹은 n을 입력하면 메시지 출력 후 프로그램 종료
            else if( cont == 'N' || cont == 'n' )
            {
                
                cout << "종료합니다.\n"; // 프로그램이 종료됨을 알림
                
                system("pause"); // Visual Studio에서 모든 출력이 끝난 후 잠시 대기
                return 0; // 프로그램 종료
            } // 문자를 입력했지만 Y(y) 혹은 N(n)을 입력하지 않았으면 오류 메시지 출력 후 재입력 유도
            else
                cout << "잘못된 문자를 입력하셨습니다. Y혹은 N중 하나로 다시 입력해주세요.\n";
        }
    } // 사용자가 2.5같은 소수점이 포함된 실수를 입력했을 때의 오류 처리에 어려움을 겪었습니다. 방법을 조금 더 강구해보겠습니다.
}
