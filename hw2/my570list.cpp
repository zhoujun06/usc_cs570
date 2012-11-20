#include "my570list.hpp"

My570ListElem::My570ListElem(void *object)
{
    obj = object;
    prev = NULL;
    next = NULL;
}

My570ListElem::~My570ListElem()
{}

My570List::My570List()
{
    num_members = 0;
    anchor.prev = &anchor;
    anchor.next = &anchor;
}

My570List::~My570List()
{}

int My570List::Append(void *obj)
{
    My570ListElem *node = new My570ListElem(obj);
    if (node == NULL) {
        return FALSE;
    }

    anchor.prev->next = node;
    node->next = &anchor;
    node->prev = anchor.prev;
    anchor.prev = node;

    num_members++;

    return TRUE;
}

int My570List::Prepend(void *obj)
{
    My570ListElem *node = new My570ListElem(obj);
    if (node == NULL) {
        return FALSE;
    }

    anchor.next->prev = node;
    node->next = anchor.next;
    node->prev = &anchor;
    anchor.next = node;

    num_members++;

    return TRUE;   
}

void My570List::Unlink(My570ListElem *elem)
{
    if (elem != NULL) {
        elem->prev->next = elem->next;
        elem->next->prev = elem->prev;
        num_members--;
        delete elem;
    }
}

void My570List::UnlinkAll()
{
    for (My570ListElem *item = First(); item != NULL; item=Next(item))
    {
        Unlink(item);
    }
}

int My570List::InsertBefore(void *obj, My570ListElem *elem)
{
    if (elem == NULL) {
        return Append(obj);
    }

    My570ListElem *node = new My570ListElem(obj);
    if (node == NULL) {
        return FALSE;
    }

    elem->prev->next = node;
    node->prev = elem->prev;
    node->next = elem;
    elem->prev = node;

    num_members++;

    return TRUE;
}


int My570List::InsertAfter(void *obj, My570ListElem *elem)
{
    if (elem == NULL) {
        return Prepend(obj);
    }

    My570ListElem *node = new My570ListElem(obj);
    if (node == NULL) {
        return FALSE;
    }

    elem->next->prev = node;
    node->next = elem->next;
    node->prev = elem;
    elem->next = node;

    num_members++;

    return TRUE;
}

My570ListElem *My570List::First()
{
    return num_members == 0 ? NULL : anchor.next;
}

My570ListElem *My570List::Last()
{
    return num_members == 0 ? NULL : anchor.prev;
}

My570ListElem *My570List::Next(My570ListElem *cur)
{
    if (cur == NULL) {
        return NULL;
    }

    return cur->next == &anchor ? NULL : cur->next;
}

My570ListElem *My570List::Prev(My570ListElem *cur)
{
    if (cur == NULL) {
        return NULL;
    }

    return cur->prev == &anchor ? NULL : cur->prev;
}

My570ListElem *My570List::Find(void *obj)
{
    for (My570ListElem *item = First(); item != NULL; item=Next(item))
    {
        if (obj == item->Obj()) {
            return item;
        }
    }

    return NULL;
}
