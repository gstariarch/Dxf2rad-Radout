/*
This file is part of

* dxf2rad - convert from DXF to Radiance scene files.
* Radout  - Export geometry from Autocad to Radiance scene files.


As a derivative work from code placed in the public domain by its
original author, this file is also in the public domain.
*/

/***********************************************************************
 * test.c -- This program tests the doubly-linked list implementation.
 *      It attempts to cover all branches of control less those taken
 *      when malloc fails.  Perhaps one day a debugging allocator can
 *      be hooked in to test those as well.
 **********************************************************************/

#ifndef PROTOTYPER
#include <stdio.h>
#include <stdlib.h>

#ifdef UNIX
	#include <unistd.h>
#endif

#include "dlltypes.h"
#include "dllproto.h"

/* These are the list operations that the program tests */

typedef enum ops {
    SDllSetup, FREEDllSetup, NEW, DESTROY, INSERT, DELETE, SEARCH,
    TRAVERSE, FIRST, LAST, NEXT, PREV, RANK, DELRANK, DATA, SETDATA,
    PUSH, PUSHF, PUSHR, POP, POPF, POPR, PEEK, PEEKF, PEEKR,
    END
} OP;

char *opnames[] = {
        "setup", "freesetup", "new", "destroy", "insert", "delete",
        "search", "traverse", "first", "last", "next", "prev", "rank",
        "delrank", "data", "setdata", "push", "pushf", "pushr", "pop",
        "popf", "popr", "peek", "peekf", "peekr", "end"
};

/* The following structure describes a test case.  An array of these
 * is given to an interpreter which performs each test and reports the
 * results.
 */
typedef struct testcase {
    OP op;                      /* Operation */
    int key;                    /* Insertion/seek key */
    int data;                   /* Application-specific or expected
                                 * data */
    int data2;                  /* Secondary data */
    int result;                 /* Return code or expected result */
    int dump;                   /* If not zero, dumps list for
                                 * debugging */
} CASE;

/* Value of "key" when calling DllDestroyList */
#define FREEKEY         1       /* call FreeKey() */
#define FREEDATA        2       /* call FreeData() */

/* Invalid value of "key" when calling the push, pop, or peek functions
*/
#define DLL_INVALID     2

/* This is the list of test cases that make up the suite. */

CASE suite[] = {
        /* OP, key, data, data2, result, dump */
        {DATA, 0, 0, 0, 0, 0},
        {SETDATA, 0, 0, 0, 0, 0},
        {DELETE, 0, 0, 0, 0, 0},
        {DELRANK, 0, 0, 0, 0, 0},
        {DESTROY, 0, 0, 0, 0, 0},
        {DESTROY, 1, 0, 0, 0, 0},
        {DESTROY, 2, 0, 0, 0, 0},
        {DESTROY, 3, 0, 0, 0, 0},
        {FIRST, 0, 0, 0, 0, 0},
        {INSERT, 0, 0, 0, 0, 0},
        {LAST, 0, 0, 0, 0, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {PEEK, 0, 0, 0, 0, 0},
        {PEEK, 0, 0, DLL_FRONT, 0, 0},
        {PEEK, 0, 0, DLL_BACK, 0, 0},
        {PEEKF, 0, 0, 0, 0, 0},
        {PEEKR, 0, 0, 0, 0, 0},
        {POP, 0, 0, 0, 0, 0},
        {POP, 0, 0, DLL_FRONT, 0, 0},
        {POP, 0, 0, DLL_BACK, 0, 0},
        {POPF, 0, 0, 0, 0, 0},
        {POPR, 0, 0, 0, 0, 0},
        {PREV, 0, 0, 0, 0, 0},
        {PUSH, 0, 0, 0, 0, 0},
        {PUSH, 0, 0, DLL_FRONT, 0, 0},
        {PUSH, 0, 0, DLL_BACK, 0, 0},
        {PUSHF, 0, 0, 0, 0, 0},
        {PUSHR, 0, 0, 0, 0, 0},
        {RANK, 0, 0, 0, 0, 0},
        {SEARCH, 0, 0, 0, 0, 0},
        {FREEDllSetup, 0, 0, 0, 0, 0},
        {TRAVERSE, 1, 0, 0, 0, 0},
        {SDllSetup, 0, 0, 0, 1, 0},
        {NEW, 0, 0, 0, 1, 0},
        {DATA, 0, 0, 0, 0, 0},
        {SETDATA, 0, 0, 0, 0, 0},
        {DELETE, 0, 0, 0, 0, 0},
        {DELRANK, 0, 0, 0, 0, 0},
        {DELRANK, -1, 0, 0, 0, 0},
        {FIRST, 0, 0, 0, 0, 0},
        {INSERT, 0, 0, 0, 0, 0},
        {LAST, 0, 0, 0, 0, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {PEEKF, 0, 0, 0, 0, 0},
        {PEEKR, 0, 0, 0, 0, 0},
        {POPF, 0, 0, 0, 0, 0},
        {POPR, 0, 0, 0, 0, 0},
        {PREV, 0, 0, 0, 0, 0},
        {RANK, 0, 0, 0, 0, 0},
        {RANK, -1, 0, 0, 0, 0},
        {SEARCH, 0, 0, 0, 0, 0},
        {TRAVERSE, 1, 0, 0, 0, 0},
        {FREEDllSetup, 0, 0, 0, 0, 0},
        {DESTROY, 0, 50, 0, 0, 0},
        {SDllSetup, 0, 23, 1, 1, 0},
        {NEW, 0, 0, 0, 1, 0},
        {INSERT, 1000, 100, 0, 1, 0},
        {INSERT, 3000, 300, 0, 1, 0},
        {INSERT, 2000, 200, 0, 1, 0},
        {INSERT, 4000, 400, 0, 1, 0},
        {INSERT, 2000, 0, 0, -1, 0},
        {INSERT, 0, 0, 0, 0, 0},
        {DATA, 0, 23, 0, 0, 0},
        {SETDATA, 0, 75, 0, 0, 0},
        {DATA, 0, 75, 0, 0, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {PREV, 0, 0, 0, 0, 0},
        {FIRST, 1000, 0, 0, 1, 0},
        {FIRST, 1000, 100, 0, 1, 0},
        {NEXT, 2000, 200, 0, 1, 0},
        {NEXT, 3000, 0, 0, 1, 0},
        {NEXT, 4000, 400, 0, 1, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {SEARCH, 1000, 100, 0, 1, 0},
        {SEARCH, 2000, 200, 0, 1, 0},
        {NEXT, 3000, 0, 0, 1, 0},
        {SEARCH, 4000, 400, 0, 1, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {SEARCH, 2500, 0, 0, 0, 0},
        {NEXT, 3000, 0, 0, 1, 0},
        {SEARCH, 500, 0, 0, 0, 0},
        {NEXT, 1000, 0, 0, 1, 0},
        {SEARCH, 4500, 0, 0, 0, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {LAST, 4000, 0, 0, 1, 0},
        {LAST, 4000, 400, 0, 1, 0},
        {PREV, 3000, 300, 0, 1, 0},
        {PREV, 2000, 0, 0, 1, 0},
        {PREV, 1000, 100, 0, 1, 0},
        {PREV, 0, 0, 0, 0, 0},
        {PREV, 0, 0, 0, 0, 0},
        {SEARCH, 3000, 300, 0, 1, 0},
        {PREV, 2000, 0, 0, 1, 0},
        {SEARCH, 1000, 100, 0, 1, 0},
        {PREV, 0, 0, 0, 0, 0},
        {SEARCH, 2500, 0, 0, 0, 0},
        {PREV, 2000, 0, 0, 1, 0},
        {SEARCH, 4500, 0, 0, 0, 0},
        {PREV, 4000, 0, 0, 1, 0},
        {SEARCH, 500, 0, 0, 0, 0},
        {PREV, 0, 0, 0, 0, 0},
        {TRAVERSE, 1, 83, 0, 4, 0},
        {TRAVERSE, 0, 0, 0, 0, 0},
        {RANK, -1, 0, 0, 0, 0},
        {RANK, 0, 100, 0, 1000, 0},
        {RANK, 0, 0, 0, 1000, 0},
        {RANK, 1, 200, 0, 2000, 0},
        {RANK, 2, 300, 0, 3000, 0},
        {RANK, 3, 400, 0, 4000, 0},
        {RANK, 4, 0, 0, 0, 0},
        {SEARCH, 2000, 200, 0, 0, 0},
        {INSERT, 6000, 600, 0, 1, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {PREV, 0, 0, 0, 0, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {DESTROY, 3, 0, 0, 5, 0},
        {NEW, 0, 0, 0, 1, 0},
        {INSERT, 1000, 100, 0, 1, 0},
        {INSERT, 2000, 200, 0, 1, 0},
        {INSERT, 3000, 300, 0, 1, 0},
        {INSERT, 4000, 400, 0, 1, 0},
        {INSERT, 5000, 500, 0, 1, 0},
        {DELETE, 500, 0, 0, 0, 0},
        {DELETE, 2500, 0, 0, 0, 0},
        {DELETE, 5500, 0, 0, 0, 0},
        {DELETE, 2000, 200, 0, 1, 0},
        {DELETE, 3000, 0, 0, 1, 0},
        {DELETE, 5000, 500, 0, 1, 0},
        {DELETE, 1000, 100, 0, 1, 0},
        {DELETE, 4000, 400, 0, 1, 0},
        {INSERT, 1000, 100, 0, 1, 0},
        {INSERT, 2000, 200, 0, 1, 0},
        {INSERT, 3000, 300, 0, 1, 0},
        {INSERT, 4000, 400, 0, 1, 0},
        {INSERT, 5000, 500, 0, 1, 0},
        {DELRANK, -1, 0, 0, 0, 0},
        {DELRANK, 5, 0, 0, 0, 0},
        {DELRANK, 1, 200, 0, 2000, 0},
        {DELRANK, 1, 0, 0, 3000, 0},
        {DELRANK, 2, 500, 0, 5000, 0},
        {DELRANK, 0, 100, 0, 1000, 0},
        {DELRANK, 0, 400, 0, 4000, 0},
        {DESTROY, 0, 0, 0, 0, 0},
        {FREEDllSetup, 0, 0, 0, 0, 0},
        {SDllSetup, 0, 0, 0, 1, 0},
        {NEW, 0, 0, 0, 1, 0},
        {PEEKF, 0, 0, 0, 0, 0},
        {PEEKR, 0, 0, 0, 0, 0},
        {PUSHF, 3000, 300, 0, 1, 0},
        {INSERT, 4000, 0, 0, 0, 0},
        {SEARCH, 3000, 0, 0, 0, 0},
        {DELETE, 4000, 0, 0, 0, 0},
        {PUSHF, 2000, 200, 0, 1, 0},
        {PEEKF, 2000, 200, 0, 1, 0},
        {PEEKF, 2000, 0, 0, 1, 0},
        {FIRST, 2000, 0, 0, 1, 0},
        {NEXT, 3000, 0, 0, 1, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {POPF, 2000, 0, 0, 1, 0},
        {POPF, 3000, 300, 0, 1, 0},
        {POPF, 0, 0, 0, 0, 0},
        {PUSHR, 2000, 200, 0, 1, 0},
        {PEEKR, 2000, 200, 0, 1, 0},
        {PEEKF, 2000, 200, 0, 1, 0},
        {PUSHR, 3000, 0, 0, 1, 0},
        {FIRST, 2000, 0, 0, 1, 0},
        {NEXT, 3000, 0, 0, 1, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {FIRST, 2000, 0, 0, 1, 0},
        {PUSHR, 4000, 0, 0, 1, 0},
        {NEXT, 0, 0, 0, 0, 0},
        {LAST, 4000, 0, 0, 1, 0},
        {PUSHF, 1000, 0, 0, 1, 0},
        {PREV, 0, 0, 0, 0, 0},
        {FIRST, 1000, 0, 0, 1, 0},
        {NEXT, 2000, 0, 0, 1, 0},
        {PEEKF, 1000, 0, 0, 1, 0},
        {PEEKR, 4000, 0, 0, 1, 0},
        {NEXT, 3000, 0, 0, 1, 0},
        {POPR, 4000, 0, 0, 1, 0},
        {POPR, 3000, 0, 0, 1, 0},
        {POPR, 2000, 200, 0, 1, 0},
        {POPR, 1000, 0, 0, 1, 0},
        {POPR, 0, 0, 0, 0, 0},
/*  OP   key   data  data2   result  dump   */
        {END, 0, 0, 0, 0, 0}
};

/* This is the comparison function */

#ifdef __STDC__
static int Comp(void *key1, void *key2)
#else
static int Comp(key1, key2)
void *key1;
void *key2;

#endif
{
    return *((int*)key1) - *((int*)key2);
}

/* This function displays a key and its data */
/* ARGSUSED */
#ifdef __STDC__
static void DumpKey(void *key, void *data, void *info)
#else
static void DumpKey(key, data, info)
void *key, *data, *info;
#endif
{
    printf("key = %4.4d, data = ", *(int*)key);
    if (data != NULL)
        printf("%4.4d\n", *(int*)data);
    else
        printf("(NULL)\n");
    return;
}

/*
 * These functions are called by DllDestroyList, and count the number of
 * key and data structures are freed, and also verify that the info
 * parameter is passed properly.  Some attempt is made to be sure that
 * the data are freed before the key.
 */
int freedKeys;                  /* Number of keys freed */
int freedData;                  /* Number of data records freed */
int *expInfo;                   /* Expected value of info */
int infoOk;                     /* Indicates that the info was always
                                 * correct */
int freeOk;                     /* Indicates that the FreeKey and
                                 * FreeData functions were called
                                 * correctly */

/* ARGSUSED */
#ifdef __STDC__
static void FreeKey(void *key, void *info)
#else
static void FreeKey(key, info)
void *key, *info;
#endif
{
    freedKeys++;
    if ((int*)info != expInfo)
        infoOk = 0;
    if ((freedData >= 0) && (freedKeys != freedData))
        freeOk = 0;
}

/* ARGSUSED */
#ifdef __STDC__
static void FreeData(void *key, void *info)
#else
static void FreeData(key, info)
void *key, *info;
#endif
{
    if ((freedKeys >= 0) && (freedKeys != freedData))
        freeOk = 0;
    freedData++;
    if ((int*)info != expInfo)
        infoOk = 0;
}

/* The following variables and the visit() function are used for testing
 * the DllTraverse() function.
 */

int lastKey;                    /* Last key encountered by DllTraverse */
int travOk;                     /* Traversal successful */
int visited;                    /* Number of nodes visited */

/* ARGSUSED */
#ifdef __STDC__
static void Visit(void *key, void *data, void *info)
#else
static void Visit(key, data, info)
void *key, *data, *info;
#endif
{
    visited++;
    if (lastKey == 0)
        lastKey = *(int*)key;
    else if (*(int*)key <= lastKey)
        travOk = 0;
    else if ((int*)info != expInfo)
        travOk = 0;
    return;
}

/* The test suite starts here... */

/* ARGSUSED */
#ifdef __STDC__
int main(int argc, char **argv)
#else
int main(argc, argv)
int argc;
char **argv;
#endif
{
    int i;                      /* Loop counter */
    int ok;                     /* Current test succeeded */
    int fail;                   /* Any test failed */
    int done;                   /* Holds size of test table */
    int intval;                 /* Integer value */
    void *ptrval;               /* Pointer value */
    void *ptrval2;              /* Pointer value */
    DLL_LIST list;               /* B-list under test */
    DLL_SETUP setup;            /* Configuration data for B-list */

    /* Initialization */
    fail = 0;
    list = (DLL_LIST)NULL;
    setup = (DLL_SETUP)NULL;

    /* Compute the number of test cases there are */
    done = sizeof(suite) / sizeof(CASE);

    /* Display heading */
    printf("test OP         key   data  data2  result  intval ptrval  ptrval2  pass\n");
    for (i = 0; (i < done) && (suite[i].op != END); i++) {
        /* Initialize test case */
        ok = 1;
        intval = 0;
        ptrval = NULL;
        ptrval2 = NULL;

        /* Perform test */
        switch (suite[i].op) {
        case SDllSetup:
            /* Test case:  data2 = 0 if no comparison function, data =
             * global data, result = 0 if NULL expected */
            setup = DllSetupList((suite[i].data2 ? Comp : (int(*)(void*, void*))NULL),
                    FreeKey, FreeData,
                    (void*)(suite[i].data ? &suite[i].data : NULL));
            ok = (suite[i].result != (setup == (DLL_SETUP)NULL));
            break;
        case FREEDllSetup:
            DllFreeSetup(setup);
            setup = (DLL_SETUP) NULL;
            break;
        case NEW:
            /* Test case:  result = 0 if NULL expected */
            list = DllNewList(setup);
            ok = (suite[i].result != (list == (DLL_LIST)NULL));
            break;
        case DESTROY:
            /* Test case:  result = number of keys to be freed, key = 0
             * if neither free fn is called, FREEKEY if FreeKey() only
             * FREEDATA if FreeData() only FREEKEY+FREEDATA if both
             * data   = info parameter */
            infoOk = 1;
            freeOk = 1;
            expInfo = &suite[i].data;
            switch (suite[i].key) {
            case 0:
                /* Fails only if dumps core or hangs */
                freedKeys = -1;
                freedData = -1;
                DllDestroyList(list, NULL, NULL, &suite[i].data);
                break;
            case FREEKEY:
                freedKeys = 0;
                freedData = -1;
                DllDestroyList(list, FreeKey, NULL, &suite[i].data);
                break;
            case FREEDATA:
                freedKeys = -1;
                freedData = 0;
                DllDestroyList(list, NULL, FreeData, &suite[i].data);
                break;
            default:
                freedKeys = 0;
                freedData = 0;
                DllDestroyList(list, FreeKey, FreeData,
                        &suite[i].data);
                break;
            }
            list = NULL;
            if (!freeOk || !infoOk)
                ok = 0;
            if ((freedKeys >= 0) && (freedKeys != suite[i].result))
                ok = 0;
            if ((freedData >= 0) && (freedData != suite[i].result))
                ok = 0;
            break;

        case INSERT:
            /* Test case:  key    = key to be inserted data   = data to
             * be stored with it result = expected val of DllInsert() */
            intval = DllInsert(list,
                    suite[i].key ? &suite[i].key : NULL,
                    suite[i].data ? &suite[i].data : NULL);
            if (intval != suite[i].result)
                ok = 0;
            break;

        case PUSHF:
            /* Test case:  key    = key to be inserted data   = data to
             * be stored with it result = 0 if failure expected */
            ptrval = (suite[i].key ? (void*)&suite[i].key : NULL);
            ptrval2 = DllPushF(list, ptrval,
                    suite[i].data ? &suite[i].data : NULL);
            if ((suite[i].result && (ptrval != ptrval2)) ||
                    (!suite[i].result && (ptrval2 != NULL)))
                ok = 0;
            break;

        case PUSHR:
            /* Test case:  key    = key to be inserted data   = data to
             * be stored with it result = 0 if failure expected */
            ptrval = (suite[i].key ? (void*)&suite[i].key : NULL);
            ptrval2 = DllPushR(list, ptrval,
                    suite[i].data ? &suite[i].data : NULL);
            if ((suite[i].result && (ptrval != ptrval2)) ||
                    (!suite[i].result && (ptrval2 != NULL)))
                ok = 0;
            break;

        case PUSH:
            /* Test case:  key    = key to be inserted data   = data to
             * be stored with it data2  = which end to push onto result
             * = 0 if failure expected */
            ptrval = (suite[i].key ? (void*)&suite[i].key : NULL);
            ptrval2 = DllPush(list, suite[i].data2, ptrval,
                    suite[i].data ? &suite[i].data : NULL);
            if ((suite[i].result && (ptrval != ptrval2)) ||
                    (!suite[i].result && (ptrval2 != NULL)))
                ok = 0;
            break;

        case DELETE:
            /* Test case:  key    = key to be deleted data   = expected
             * data returned by DllDelete result = 0 if failure
             * expected */
            ptrval2 = DllDelete(list,
                    suite[i].key ? &suite[i].key : NULL,
                    suite[i].data ? &ptrval : NULL);
            if (suite[i].key == 0) {
                if (ptrval2 != NULL)
                    ok = 0;
            } else {
                if (ptrval2 == NULL) {
                    if (suite[i].result)
                        ok = 0;
                } else {
                    if (*(int*)ptrval2 != suite[i].key)
                        ok = 0;
                }
            }
            if (suite[i].data != 0) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case SEARCH:
            /* Test case:  key    = key to be sought data   = expected
             * data returned by DllSearch result = 0 if failure
             * expected */
            ptrval2 = DllSearch(list,
                    suite[i].key ? &suite[i].key : NULL,
                    suite[i].data ? &ptrval : NULL);
            if (suite[i].key == 0) {
                if (ptrval2 != NULL)
                    ok = 0;
            } else {
                if (ptrval2 == NULL) {
                    if (suite[i].result)
                        ok = 0;
                } else {
                    if (*(int*)ptrval2 != suite[i].key) {
                        ok = 0;
                    }
                }
            }
            if (suite[i].data != 0) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case TRAVERSE:
            /* Test case:  data   = info passed to DllTraverse key = 0
             * if NULL passed as fn result = the expected number of
             * times visit() is called */
            visited = 0;
            travOk = 1;
            expInfo = &suite[i].data;
            lastKey = 0;
            DllTraverse(list, (suite[i].key ? Visit : (void (*)(void*,void*,void*))NULL),
                    &suite[i].data);
            ok = travOk;
            if (visited != suite[i].result)
                ok = 0;
            break;

        case FIRST:
            /* Test case:  key    = expected key returned data   =
             * expected data result = 0 if list is empty */
            ptrval2 = DllFirst(list,
                    suite[i].data ? &ptrval : NULL);
            if (suite[i].result) {
                if (*(int*)ptrval2 != suite[i].key)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case LAST:
            /* Test case:  key    = expected key returned data   =
             * expected data result = 0 if list is empty */
            ptrval2 = DllLast(list,
                    suite[i].data ? &ptrval : NULL);
            if (suite[i].result) {
                if (*(int*)ptrval2 != suite[i].key)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case NEXT:
            /* Test case:  key    = expected key returned data   =
             * expected data result = 0 if last key was found */
            ptrval2 = DllNext(list,
                    suite[i].data ? &ptrval : NULL);
            if (suite[i].result) {
                if (*(int*)ptrval2 != suite[i].key)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case PREV:
            /* Test case:  key    = expected key returned data   =
             * expected data result = 0 if first key was found */
            ptrval2 = DllPrev(list,
                    suite[i].data ? &ptrval : NULL);
            if (suite[i].result) {
                if (*(int*)ptrval2 != suite[i].key)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case RANK:
            /* Test case:  key    = rank searched for data   = expected
             * data result = expected key */
            ptrval2 = DllRank(list, suite[i].key,
                    suite[i].data ? &ptrval : NULL);
            if (suite[i].result != 0) {
                if (*(int*)ptrval2 != suite[i].result)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data != 0) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            }
            break;

        case DELRANK:
            /* Test case:  key    = rank to be deleted data   =
             * expected data result = expected key */
            ptrval2 = DllDelRank(list, suite[i].key,
                    suite[i].data ? &ptrval : NULL);
            if (suite[i].result != 0) {
                if (*(int*)ptrval2 != suite[i].result)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data != 0) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            }
            break;

        case DATA:
            /* Test case:  data = expected data */
            ptrval = DllGetListData(list);
            if (suite[i].data != 0) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case SETDATA:
            /* Test case:  data = new data */
            DllSetListData(list, &suite[i].data);
            break;

        case POPF:
            /* Test case:  key    = expected key returned data   =
             * expected data result = 0 if list is empty */
            ptrval2 = DllPopF(list, suite[i].data ? &ptrval : NULL);
            if (suite[i].result) {
                if (*(int*)ptrval2 != suite[i].key)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case POPR:
            /* Test case:  key    = expected key returned data   =
             * expected data result = 0 if list is empty */
            ptrval2 = DllPopR(list, suite[i].data ? &ptrval : NULL);
            if (suite[i].result) {
                if (*(int*)ptrval2 != suite[i].key)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case POP:
            /* Test case:  key    = expected key returned data   =
             * expected data data2  = end to pop from result = 0 if
             * list is empty */
            ptrval2 = DllPop(list, suite[i].data2,
                    suite[i].data ? &ptrval : NULL);
            if (suite[i].result) {
                if (*(int*)ptrval2 != suite[i].key)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case PEEKF:
            /* Test case:  key    = expected key returned data   =
             * expected data result = 0 if list is empty */
            ptrval2 = DllPeekF(list,
                    suite[i].data ? &ptrval : NULL);
            if (suite[i].result) {
                if (*(int*)ptrval2 != suite[i].key)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case PEEKR:
            /* Test case:  key    = expected key returned data   =
             * expected data result = 0 if list is empty */
            ptrval2 = DllPeekR(list, suite[i].data ? &ptrval : NULL);
            if (suite[i].result) {
                if (*(int*)ptrval2 != suite[i].key)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        case PEEK:
            /* Test case:  key    = expected key returned data   =
             * expected data data2  = end to peek into result = 0 if
             * list is empty */
            ptrval2 = DllPeek(list, suite[i].data2,
                    suite[i].data ? &ptrval : NULL);
            if (suite[i].result) {
                if (*(int*)ptrval2 != suite[i].key)
                    ok = 0;
            } else {
                if (ptrval2 != NULL)
                    ok = 0;
            }
            if (suite[i].data) {
                if (*(int*)ptrval != suite[i].data)
                    ok = 0;
            } else {
                if (ptrval != NULL)
                    ok = 0;
            }
            break;

        default:
            break;
        }

        /* Note test case failure */
        if (!ok)
            fail = 1;

        /* Display result of test case */
        printf("%4.4d %-9.9s %5.4d %5.4d %5.4d  %5.4d   %5.4d   ",
                i, opnames[(int) suite[i].op], suite[i].key, suite[i].data,
                suite[i].data2, suite[i].result, intval);
        if (ptrval == NULL)
            printf(" NULL   ");
        else
            printf("%5.4d   ", *(int*)ptrval);
        if (ptrval2 == NULL)
            printf(" NULL    ");
        else
            printf("%5.4d    ", *(int*)ptrval2);
        if (ok)
            printf("yes  ");
        else
            printf("no   ");
        printf("\n");

        /* Dump the list if requested */
        if ((suite[i].dump) && (list != NULL)) {
            printf("Contents of list:\n");
            DllDump(list, DumpKey, NULL, stderr);
        }
    }

    /* Display summary */
    printf("TEST %s\n", (fail ? "FAILED" : "PASSED"));

    /* Return 0 on success, non-zero on failure */
    return ((i != done) && !fail);
}

void SmlWarn(s1, s2)
char *s1, s2;
{
    fprintf(stderr, s1, s2);
    fprintf(stderr, "\n");
}
#endif /* PROTOTYPER */
/********* End of file *********/
