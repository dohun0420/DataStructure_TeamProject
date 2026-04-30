#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#include <malloc.h>
#include <assert.h>
#include <random>

using std::getline;
using std::string;
using std::cin;


enum class Kategorie
{
    Default = 0,
    weapon,
    armor,
    potion,
    accessory,
    end,
};

typedef struct Item
{
    unsigned int iItemCount;      // 아이템 개수
    string sItemName;             // 아이템 이름
    Kategorie eKategorie;         // 아이템 카테고리
    char Rank;                    // 아이템 등급
} Item;

typedef struct Inventory
{
    Item* pEle;
    int iCount;
    int iMaxCount;
} ArrInven;

// 인벤토리 배열 초기화
void InitArr(ArrInven* _pArr)
{
    _pArr->pEle = new Item[10];
    _pArr->iCount = 0;
    _pArr->iMaxCount = 10;
}

void Reallocate(ArrInven* _pArr);

void PushBack(ArrInven* _pArr, Item _item)
{
    // 힙 영역에 할당한 공간이 다 찼을 경우
    if (_pArr->iMaxCount <= _pArr->iCount)
    {
        // 재할당
        Reallocate(_pArr);
    }
    // 만약 원래 있던 데이터라면(= sItemName으로 판단), 수량만 증가
    for (int i = 0; i < _pArr->iCount; ++i)
    {
        if (_pArr->pEle[i].sItemName == _item.sItemName)
        {
            ++(_pArr->pEle[i].iItemCount);
            return;
        }
    }
    // 원래 있던 아이템이 아니라면 
    // 데이터 추가, 총 데이터 개수 증가
    _pArr->pEle[_pArr->iCount++] = _item;
}

// 동적 할당 공간 부족 시 재할당
void Reallocate(ArrInven* _pArr)
{
    // 1. 새로 할당받은 공간의 주소를 저장할 New 포인터 변수 선언 - 새로 할당받을 공간은 maxCount*2
    int iResizeCount = _pArr->iMaxCount * 2;
    Item* pNew = new Item[iResizeCount];

    assert(pNew);  // 할당 실패시 경고 표시

    // 2. 기존 배열 공간에 있던 데이터들을 새 배열 공간으로 복사
    for (int i = 0; i < _pArr->iCount; ++i)
    {
        pNew[i] = _pArr->pEle[i];
    }
    // 3. 기존 배열 공간 메모리 해제
    delete[](_pArr->pEle);

    // 4. 배열이 새로 할당된 공간을 가리키도록 한다.
    _pArr->pEle = pNew;

    // 5. iMaxCount 변경점 적용
    _pArr->iMaxCount *= 2;
}

// 인벤토리 해제
void ReleaseArr(ArrInven* _pArr)
{
    delete[](_pArr->pEle);
    _pArr->iCount = 0;
    _pArr->iMaxCount = 0;
}

// 이름으로 탐색 - 인덱스 반환
int FindAsName(ArrInven* _pArr, string _s)
{
    for (int i = 0; i < _pArr->iCount; ++i)
    {
        if (_pArr->pEle[i].sItemName == _s)
        {
            return i;
        }
    }
    return -1;
}

// 인벤토리 출력
void PrintInven(ArrInven* _pArr)
{
    if (_pArr->iCount == 0)
    {
        printf("인벤토리가 비었습니다.\n");
        return;
    }
    // 출력
    printf("============================현재 인벤토리 내용============================\n");
    string s;
    for (int i = 0; i < _pArr->iCount; ++i)
    {
        switch (_pArr->pEle[i].eKategorie)
        {
        case Kategorie::Default:
            s = "Default";
            break;
        case Kategorie::weapon:
            s = "weapon";
            break;
        case Kategorie::armor:
            s = "armor";
            break;
        case Kategorie::potion:
            s = "potion";
            break;
        case Kategorie::accessory:
            s = "accessory";
            break;
        case Kategorie::end:
            s = "end";
            break;
        default:
            break;
        }
        printf(" [%s]\t\t항목: %s\t\t수량: %d   \t - [%d]\n", _pArr->pEle[i].sItemName.c_str(), s.c_str(), _pArr->pEle[i].iItemCount, i + 1);
    }
    printf("==========================================================================\n\n");
}

void DeleteItem(ArrInven* _Inven)
{
    string del;
    std::cout << "삭제하고 싶은 아이템 작성 : ";
    cin.ignore();  // 스트림 버퍼 비움
    getline(cin, del);
    int idx = FindAsName(_Inven, del);
    if (idx != -1)
    {
        for (int i = idx; i < _Inven->iCount - 1; ++i)
        {
            _Inven->pEle[i] = _Inven->pEle[i + 1];
        }
        --_Inven->iCount;
        printf("아이템이 삭제되었습니다.\n");
    }
    else
    {
        printf("삭제하고자 하는 아이템이 존재하지 않습니다.\n");
    }
}

void InputItem(ArrInven* _Inven)
{
    Item NewItem;
    string sInput_item;
    string input_rank;
    int iKategorie = 0;
    bool bLoop = true;

    // 1. 아이템 이름 등록
    printf("삽입하고 싶은 아이템 이름 : ");
    cin.ignore(); // 개행 문자를 무시
    getline(cin, sInput_item);
    NewItem.sItemName = sInput_item;

    while (bLoop)
    {
        // 2. 아이템 등급 등록
        printf("삽입하고 싶은 아이템 등급(A / B / C) : ");
        getline(cin, input_rank);
        if (input_rank[0] == 'a' || input_rank[0] == 'b' || input_rank[0] == 'c' ||
            input_rank[0] == 'A' || input_rank[0] == 'B' || input_rank[0] == 'C')
            break;
        else
            printf("아이템 등급을 소문자/대문자 A, B, C 중 하나로 작성해 주세요.\n");
    }
    NewItem.Rank = input_rank[0];

    while (bLoop)
    {
        // 3. 아이템 카테고리 등록
        printf("[카테고리 목록]\n1. weapon\n2. armor\n3. potion\n4. accessories\n");
        printf("삽입하고 싶은 아이템 카테고리 번호를 입력해 주세요 : ");
        cin >> iKategorie;
        if (iKategorie == 1 || iKategorie == 2 || iKategorie == 3 || iKategorie == 4)
            break;
        else
            printf("아이템 카테고리의 번호를 1, 2, 3, 4 중 하나로 작성해 주세요.\n");
    }
    NewItem.eKategorie = static_cast<Kategorie>(iKategorie);
    NewItem.iItemCount = 1;
    PushBack(_Inven, NewItem);

    string sKate;
    switch (NewItem.eKategorie)
    {
    case Kategorie::weapon:
        sKate = "weapon";
        break;
    case Kategorie::armor:
        sKate = "armor";
        break;
    case Kategorie::potion:
        sKate = "potion";
        break;
    case Kategorie::accessory:
        sKate = "accessory";
        break;
    default:
        break;
    }

    printf("[아이템 : %s, 등급 : %c, 카테고리 : %s]가 추가되었습니다.\n",
        NewItem.sItemName.c_str(), NewItem.Rank, sKate.c_str());
    int idx = FindAsName(_Inven, NewItem.sItemName);
    printf("현재 수량 : %d\n", _Inven->pEle[idx].iItemCount);
}

// 카테고리 정렬 함수
bool CompareByKategorie(const Item& a, const Item& b)
{
    return static_cast<int>(a.eKategorie) < static_cast<int>(b.eKategorie);
}

// 아이템 개수 정렬 함수
bool CompareByItemCount(const Item& a, const Item& b)
{
    return a.iItemCount > b.iItemCount;
}

// 랭크별 정렬 함수
bool CompareByRank(const Item& a, const Item& b)
{
    return a.Rank < b.Rank; // 등급 'A', 'B', 'C' 순서로 정렬
}

// 이름순 정렬 함수
bool CompareByName(const Item& a, const Item& b)
{
    return a.sItemName < b.sItemName;
}

void SelectSorting(ArrInven* _Inven)
{
    bool bExit = true;
    while (bExit)
    {
        printf("원하는 정렬을 선택하세요 - 'c':카테고리 정렬, 'a':수량순 정렬, 'r':등급별 정렬, 'n':이름순 정렬\n");
        string c;
        cin >> c;
        switch (c[0])
        {
        case 'c':
            printf("카테고리 정렬을 실시합니다. 카테고리는 Weapon-armor-potion-accessories 순서로 정렬합니다.\n\n");
            // 카테고리 정렬
            std::sort(_Inven->pEle, _Inven->pEle + _Inven->iCount, CompareByKategorie);
            bExit = false;
            break;
        case 'a':
            printf("수량순 정렬을 실시합니다. 가장 많은 아이템 순서로 정렬합니다.\n\n");
            // 수량순 정렬
            std::sort(_Inven->pEle, _Inven->pEle + _Inven->iCount, CompareByItemCount);
            bExit = false;
            break;
        case 'r':
            printf("등급별 정렬을 실시합니다. 가장 높은 등급(A) 순서로 정렬합니다.\n\n");
            // 등급별 정렬
            std::sort(_Inven->pEle, _Inven->pEle + _Inven->iCount, CompareByRank);
            bExit = false;
            break;
        case 'n':
            printf("이름순 정렬을 실시합니다. 'a'~'z'순서로 정렬합니다.\n\n");
            // 이름순 정렬
            std::sort(_Inven->pEle, _Inven->pEle + _Inven->iCount, CompareByName);
            bExit = false;
            break;
        default:
            printf("올바른 문자를 입력해 주세요\n");
            break;
        }
    }
}

void InvenProcess(ArrInven* _Inven)
{
    int iCom;
    bool bLoop = true;
    while (bLoop)
    {
        printf("======================================원하시는 기능을 선택해주세요======================================\n");
        printf("|  1 : 아이템 정렬하기 / 2 : 아이템 삭제하기 / 3 : 아이템 삽입하기 / 4 : 인벤토리 출력하기 / 5 : 종료  |\n");
        printf("========================================================================================================\n");
        printf("[선택] : ");
        cin >> iCom;
        if (iCom >= INT_MAX)
            cin.clear();
        else if (cin.fail())
        {
            cin.clear();
            cin.ignore(10);
        }

        switch (iCom)
        {
        case 1:
            SelectSorting(_Inven);
            break;
        case 2:
            DeleteItem(_Inven);
            break;
        case 3:
            InputItem(_Inven);
            break;
        case 4:
            PrintInven(_Inven);
            break;
        case 5:
            bLoop = false;
            break;
        default:
            printf("*선택하신 명령어를 다시 한번 확인해 주세요*\n");
            break;
        }
    }
}

int main()
{
    // 배열 버전 인벤토리 생성
    ArrInven I;

    srand(300);
    // 초기화
    InitArr(&I);

    Item Weapon;
    Item Armor;
    Item Potion;
    Item accessorie;

    // 무기 삽입
    Weapon.eKategorie = Kategorie::weapon;
    Weapon.iItemCount = rand() % 300 + 1;
    Weapon.sItemName = "Blade";
    Weapon.Rank = 'C';
    PushBack(&I, Weapon);

    Weapon.eKategorie = Kategorie::weapon;
    Weapon.iItemCount = rand() % 300 + 1;
    Weapon.sItemName = "Big blade";
    Weapon.Rank = 'A';
    PushBack(&I, Weapon);

    Weapon.eKategorie = Kategorie::weapon;
    Weapon.iItemCount = rand() % 300 + 1;
    Weapon.sItemName = "Hand Gun";
    Weapon.Rank = 'B';
    PushBack(&I, Weapon);

    Weapon.eKategorie = Kategorie::weapon;
    Weapon.iItemCount = rand() % 300 + 1;
    Weapon.sItemName = "Shout Gun";
    PushBack(&I, Weapon);

    Weapon.eKategorie = Kategorie::weapon;
    Weapon.iItemCount = rand() % 300 + 1;
    Weapon.sItemName = "Hammer";
    Weapon.Rank = 'B';
    PushBack(&I, Weapon);

    // 방어구 삽입
    Armor.eKategorie = Kategorie::armor;
    Armor.iItemCount = rand() % 300 + 1;
    Armor.sItemName = "Metal Plate";
    Armor.Rank = 'A';
    PushBack(&I, Armor);

    Armor.eKategorie = Kategorie::armor;
    Armor.iItemCount = rand() % 300 + 1;
    Armor.sItemName = "Broken Plate";
    Armor.Rank = 'C';
    PushBack(&I, Armor);

    Armor.eKategorie = Kategorie::armor;
    Armor.iItemCount = rand() % 300 + 1;
    Armor.sItemName = "Wood Plate";
    Armor.Rank = 'B';
    PushBack(&I, Armor);

    // 물약 삽입
    Potion.eKategorie = Kategorie::potion;
    Potion.iItemCount = rand() % 300 + 1;
    Potion.sItemName = "Red Potion";
    Potion.Rank = 'A';
    PushBack(&I, Potion);

    Potion.eKategorie = Kategorie::potion;
    Potion.iItemCount = rand() % 300 + 1;
    Potion.sItemName = "Blue Potion";
    Potion.Rank = 'B';
    PushBack(&I, Potion);

    Potion.eKategorie = Kategorie::potion;
    Potion.iItemCount = rand() % 300 + 1;
    Potion.sItemName = "Green Potion";
    Potion.Rank = 'C';
    PushBack(&I, Potion);

    // 장신구 삽입
    accessorie.eKategorie = Kategorie::accessory;
    accessorie.iItemCount = rand() % 300 + 1;
    accessorie.sItemName = "Bracelet";
    accessorie.Rank = 'B';
    PushBack(&I, accessorie);

    accessorie.eKategorie = Kategorie::accessory;
    accessorie.iItemCount = rand() % 300 + 1;
    accessorie.sItemName = "Necklace";
    accessorie.Rank = 'C';
    PushBack(&I, accessorie);

    accessorie.eKategorie = Kategorie::accessory;
    accessorie.iItemCount = rand() % 300 + 1;
    accessorie.sItemName = "Pendant";
    accessorie.Rank = 'A';
    PushBack(&I, accessorie);

    PrintInven(&I);

    InvenProcess(&I);

    ReleaseArr(&I);

    return 0;
}