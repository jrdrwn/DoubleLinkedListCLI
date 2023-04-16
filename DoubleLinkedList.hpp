#include <algorithm>
#include <iostream>
#include <random>
#include <string>

using namespace std;

struct Perpustakaan {
  string id, judul, pengarang, tahun, jumlah;
};

struct node {
  Perpustakaan data;
  node *next, *prev;
};

node *head, *tail;

void tambahData(node **head, Perpustakaan data) {
  node *newNode = new node;
  newNode->data = data;
  newNode->next = newNode->prev = NULL;

  if (*head == NULL) {
    *head = newNode;
  } else {
    node *curr = *head;
    while (curr->next != NULL) {
      curr = curr->next;
    }
    curr->next = newNode;
    newNode->prev = curr;
  }
}

void hapusData(node **head, string input) {
  node *curr = *head;
  while (curr != NULL) {
    if (curr->data.judul == input || curr->data.pengarang == input ||
        curr->data.tahun == input || curr->data.jumlah == input ||
        curr->data.id == input) {
      if (curr->prev == NULL) {
        *head = curr->next;
        if (curr->next != NULL) {
          curr->next->prev = NULL;
        }
      } else if (curr->next == NULL) {
        curr->prev->next = NULL;
      } else {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
      }
      delete curr;
      break;
    }
    curr = curr->next;
  }
}

void updateData(node *head, string id, Perpustakaan data) {
  node *curr = head;
  while (curr != NULL) {
    if (curr->data.id == id) {
      data.id = id;
      curr->data = data;
      break;
    }
    curr = curr->next;
  }
}

bool isPatternInData(Perpustakaan data, string pattern) {
  return (data.judul.find(pattern) != string::npos ||
          data.pengarang.find(pattern) != string::npos ||
          data.tahun.find(pattern) != string::npos ||
          data.jumlah.find(pattern) != string::npos ||
          data.id.find(pattern) != string::npos);
}

bool isEmptyData(Perpustakaan data, bool single = false) {
  if (single)
    return data.id.length() > 0;
  else
    return (data.judul.length() > 0 && data.pengarang.length() > 0 &&
            data.tahun.length() > 0 && data.jumlah.length() > 0);
}

void urutData(node *head) {
  if (head == NULL) return;
  int jarak = 1;
  bool sudah = true;
  while (jarak <= head->data.id.length() / 3) {
    jarak = jarak * 3 + 1;
  }
  while (jarak > 0) {
    sudah = true;
    node *curr = head;
    while (curr->next != NULL) {
      if (curr->data.id.compare(curr->next->data.id) > 0) {
        Perpustakaan temp = curr->data;
        curr->data = curr->next->data;
        curr->next->data = temp;
        sudah = false;
      }
      curr = curr->next;
    }
    if (sudah) {
      jarak = jarak / 3;
    }
  }
}

string random_string() {
  string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

  random_device rd;
  mt19937 generator(rd());
  shuffle(str.begin(), str.end(), generator);

  return str.substr(0, 5);
}