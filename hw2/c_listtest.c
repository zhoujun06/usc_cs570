/*
 * Author:      William Chia-Wei Cheng (bill.cheng@acm.org)
 *
 * @(#)$Id: c_listtest.c,v 1.1 2012/08/23 19:29:41 william Exp $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "cs570.h"

#include "my570list.h"

static char gszProgName[MAXPATHLENGTH];

int gnDebug=0;

/* ----------------------- Utility Functions ----------------------- */

static
void Usage()
{
    fprintf(stderr,
            "usage: %s %s\n",
            gszProgName, "test");
    exit(-1);
}

static
void ProcessOptions(int argc, char *argv[])
{
    for (argc--, argv++; argc > 0; argc--, argv++) {
        if (*argv[0] == '-') {
            if (strcmp(*argv, "-debug") == 0) {
                gnDebug++;
            } else {
                Usage();
            }
        } else {
            Usage();
        }
    }
}

static
void SetProgramName(char *s)
{
    char *c_ptr=strrchr(s, DIR_SEP);

    if (c_ptr == NULL) {
        strcpy(gszProgName, s);
    } else {
        strcpy(gszProgName, ++c_ptr);
    }
}

/* ----------------------- Test() ----------------------- */

static
void CreateTestList(My570List *pList, int num_items)
{
    int i=0;

    for (i=0; i < num_items; i++) {
        (void)My570ListAppend(pList, (void*)i);
    }
}

static
int RandomIndex(int num_values)
    /* return a random value >= 0 and <= num_values-1 */
{
    double dval=(drand48()*((double)num_values))-((double)0.5);
    int idx=0;

    if (dval < ((double)(-0.5))) dval = (double)(-0.5);
    idx = round(dval);
    if (idx < 0) idx = 0;
    if (idx >= num_values) idx = num_values-1;

    return idx;
}

static
void RandomShuffle(My570List *pList, int num_items)
{
    int i=0;
    My570List list2;
    My570ListElem *elem=NULL;

    memset(&list2, 0, sizeof(My570List));
    (void)My570ListInit(&list2);

    for (i=0; i < num_items; i++) {
        int j=0, idx=0, num_in_list=num_items-i;
        void *obj=NULL;

        idx = RandomIndex(num_in_list);
        for (elem=My570ListFirst(pList); elem != NULL; elem=My570ListNext(pList, elem)) {
            if (j == idx) {
                break;
            }
            j++;
        }
        if (elem == NULL) {
            fprintf(stderr, "Unrecoverable error in RandomShuffle().\n");
            exit(1);
        }
        obj = elem->obj;
        My570ListUnlink(pList, elem);
        (void)My570ListAppend(&list2, obj);
    }
    if (!My570ListEmpty(pList)) {
        fprintf(stderr, "List not empty in RandomShuffle().\n");
        exit(1);
    }
    for (elem=My570ListFirst(&list2); elem != NULL; elem=My570ListNext(&list2, elem)) {
        (void)My570ListAppend(pList, elem->obj);
    }
}

static
void FindAllInList(My570List *pList, int num_items)
{
    int i=0;

    for (i=0; i < num_items; i++) {
        if (My570ListFind(pList, (void*)i) == NULL) {
            fprintf(stderr, "Cannot find %1d in FindAllInList().\n", i);
        }
    }
}

static
void PrintTestList(My570List *pList, int num_items)
{
    My570ListElem *elem=NULL;

    if (My570ListLength(pList) != num_items) {
        fprintf(stderr, "List length is not %1d in PrintTestList().\n", num_items);
        exit(1);
    }
    for (elem=My570ListFirst(pList); elem != NULL; elem=My570ListNext(pList, elem)) {
        int ival=(int)(elem->obj);

        fprintf(stdout, "%1d ", ival);
    }
    fprintf(stdout, "\n");
}

static
void CopyTestList(My570List *pListFrom, My570List *pListTo)
{
    My570ListElem *elem=NULL;

    for (elem=My570ListFirst(pListFrom); elem != NULL; elem=My570ListNext(pListFrom, elem)) {
        (void)My570ListAppend(pListTo, elem->obj);
    }
}

static
void BubbleForward(My570List *pList, My570ListElem **pp_elem1, My570ListElem **pp_elem2)
    /* (*pp_elem1) must be closer to First() than (*pp_elem2) */
{
    My570ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1=elem1->obj, *obj2=elem2->obj;
    My570ListElem *elem1prev=My570ListPrev(pList, elem1);
/*  My570ListElem *elem1next=My570ListNext(pList, elem1); */
/*  My570ListElem *elem2prev=My570ListPrev(pList, elem2); */
    My570ListElem *elem2next=My570ListNext(pList, elem2);

    My570ListUnlink(pList, elem1);
    My570ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My570ListPrepend(pList, obj2);
        *pp_elem1 = My570ListFirst(pList);
    } else {
        (void)My570ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1 = My570ListNext(pList, elem1prev);
    }
    if (elem2next == NULL) {
        (void)My570ListAppend(pList, obj1);
        *pp_elem2 = My570ListLast(pList);
    } else {
        (void)My570ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2 = My570ListPrev(pList, elem2next);
    }
}

static
void BubbleSortForwardList(My570List *pList, int num_items)
{
    My570ListElem *elem=NULL;
    int i=0;

    if (My570ListLength(pList) != num_items) {
        fprintf(stderr, "List length is not %1d in BubbleSortForwardList().\n", num_items);
        exit(1);
    }
    for (i=0; i < num_items; i++) {
        int j=0, something_swapped=FALSE;
        My570ListElem *next_elem=NULL;

        for (elem=My570ListFirst(pList), j=0; j < num_items-i-1; elem=next_elem, j++) {
            int cur_val=(int)(elem->obj), next_val=0;

            next_elem=My570ListNext(pList, elem);
            next_val = (int)(next_elem->obj);

            if (cur_val > next_val) {
                BubbleForward(pList, &elem, &next_elem);
                something_swapped = TRUE;
            }
        }
        if (!something_swapped) break;
    }
}

static
void BubbleBackward(My570List *pList, My570ListElem **pp_elem1, My570ListElem **pp_elem2)
    /* (*pp_elem1) must be closer to Last() than (*pp_elem2) */
{
    My570ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1=elem1->obj, *obj2=elem2->obj;
/*  My570ListElem *elem1prev=My570ListPrev(pList, elem1); */
    My570ListElem *elem1next=My570ListNext(pList, elem1);
    My570ListElem *elem2prev=My570ListPrev(pList, elem2);
/*  My570ListElem *elem2next=My570ListNext(pList, elem2); */

    My570ListUnlink(pList, elem1);
    My570ListUnlink(pList, elem2);
    if (elem1next == NULL) {
        (void)My570ListAppend(pList, obj2);
        *pp_elem1 = My570ListLast(pList);
    } else {
        (void)My570ListInsertBefore(pList, obj2, elem1next);
        *pp_elem1 = My570ListPrev(pList, elem1next);
    }
    if (elem2prev == NULL) {
        (void)My570ListPrepend(pList, obj1);
        *pp_elem2 = My570ListFirst(pList);
    } else {
        (void)My570ListInsertAfter(pList, obj1, elem2prev);
        *pp_elem2 = My570ListNext(pList, elem2prev);
    }
}

static
void BubbleSortBackwardList(My570List *pList, int num_items)
{
    My570ListElem *elem=NULL;
    int i=0;

    if (My570ListLength(pList) != num_items) {
        fprintf(stderr, "List length is not %1d in BubbleSortForwardList().\n", num_items);
        exit(1);
    }
    for (i=0; i < num_items; i++) {
        int j=0, something_swapped=FALSE;
        My570ListElem *prev_elem=NULL;

        for (elem=My570ListLast(pList), j=0; j < num_items-i-1; elem=prev_elem, j++) {
            int cur_val=(int)(elem->obj), prev_val=0;

            prev_elem=My570ListPrev(pList, elem);
            prev_val = (int)(prev_elem->obj);

            if (cur_val < prev_val) {
                BubbleBackward(pList, &elem, &prev_elem);
                something_swapped = TRUE;
            }
        }
        if (!something_swapped) break;
    }
}
static
void CompareTestList(My570List *pList1, My570List *pList2, int num_items)
{
    My570ListElem *elem1=NULL, *elem2=NULL;
    int idx=0;

    if (My570ListLength(pList1) != num_items) {
        fprintf(stderr, "List1 length is not %1d in CompareTestList().\n", num_items);
        exit(1);
    }
    if (My570ListLength(pList2) != num_items) {
        fprintf(stderr, "List2 length is not %1d in CompareTestList().\n", num_items);
        exit(1);
    }
    for (elem1=My570ListFirst(pList1), elem2=My570ListFirst(pList2);
            elem1 != NULL && elem2 != NULL;
            elem1=My570ListNext(pList1, elem1), elem2=My570ListNext(pList2, elem2), idx++) {
        int ival1=(int)(elem1->obj);
        int ival2=(int)(elem2->obj);

        if (ival1 != ival2) {
            fprintf(stderr, "(%1d,%1d): item %1d not identical in CompareTestList().\n", ival1, ival2, idx);
            exit(1);
        }
    }
}

static
void DoTest()
{
    int num_items=64;
    My570List list, list2;

    memset(&list, 0, sizeof(My570List));
    memset(&list2, 0, sizeof(My570List));
    (void)My570ListInit(&list);
    (void)My570ListInit(&list2);

    CreateTestList(&list, num_items);
    RandomShuffle(&list, num_items);
    FindAllInList(&list, num_items);
    CopyTestList(&list, &list2);

    BubbleSortForwardList(&list, num_items);
    if (gnDebug > 0) PrintTestList(&list, num_items);

    BubbleSortBackwardList(&list2, num_items);
    if (gnDebug > 0) PrintTestList(&list2, num_items);

    CompareTestList(&list, &list2, num_items);

    My570ListUnlinkAll(&list);
    My570ListUnlinkAll(&list2);
}

/* ----------------------- Process() ----------------------- */

static
void Process()
{
    int i=0, num_itr=10;

    if (gnDebug > 0) {
        srand48(gnDebug);
    } else {
        struct timeval tv;

        (void)gettimeofday(&tv, NULL);
        srand48(((long)tv.tv_sec)+((long)tv.tv_usec));
    }
    for (i=0; i < num_itr; i++) {
        DoTest();
    }
}

/* ----------------------- main() ----------------------- */

int main(int argc, char *argv[])
{
    SetProgramName(*argv);
    ProcessOptions(argc, argv);

    Process();
    return(0);
}
