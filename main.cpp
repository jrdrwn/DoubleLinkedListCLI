#include "DoubleLinkedList.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/table.hpp"
#include "ftxui/screen/screen.hpp"
#include "scroller.hpp"

using namespace ftxui;

auto createTableData(node *head, string pattern) {
  auto textCenter = [](string s) { return text(s) | hcenter; };
  vector<vector<Element>> table_data{
      {textCenter("ID"), textCenter("Judul"), textCenter("Pengarang"),
       textCenter("Tahun"), textCenter("Jumlah")}};
  node *curr = head;
  while (curr != NULL) {
    auto data = {textCenter(curr->data.id), textCenter(curr->data.judul),
                 textCenter(curr->data.pengarang), textCenter(curr->data.tahun),
                 textCenter(curr->data.jumlah)};
    if (pattern.length() < 1) {
      table_data.push_back(data);
    } else if (isPatternInData(curr->data, pattern)) {
      table_data.push_back(data);
    }
    curr = curr->next;
  }
  return table_data;
}

int main() {
  Perpustakaan data;
  auto judul = Input(&data.judul, "judul");
  auto pengarang = Input(&data.pengarang, "pengarang");
  auto tahun = Input(&data.tahun, "tahun");
  auto jumlah = Input(&data.jumlah, "jumlah");

  string pattern;
  auto input_pattern = Input(&pattern, "kata kunci");

  auto tambahBtn = Button("Tambah",
                          [&] {
                            if (isEmptyData(data)) {
                              data.id = random_string();
                              tambahData(&head, data);
                              data = {};
                            }
                          }) |
                   color(Color::Green);
  auto perbaruiBtn = Button("Perbarui",
                            [&] {
                              if (isEmptyData(data, true)) {
                                updateData(head, pattern, data);
                                data = {};
                                pattern = "";
                              }
                            }) |
                     color(Color::Blue) | flex;
  auto hapusBtn = Button("Hapus",
                         [&] {
                           hapusData(&head, pattern);
                           pattern = "";
                         }) |
                  color(Color::Red) | flex;

  auto sortirBtn = Button("Urut (Shell Sort) (ASC)", [&] { urutData(head); }) |
                   color(Color::CadetBlue);

  auto rendererChild =
      Container::Vertical({judul, pengarang, tahun, jumlah, input_pattern,
                           tambahBtn, perbaruiBtn, hapusBtn, sortirBtn});
  auto actions = Renderer(rendererChild, [&] {
    return vbox({hbox(text("Judul : "), judul->Render()),
                 hbox(text("Pengarang : "), pengarang->Render()),
                 hbox(text("Tahun : "), tahun->Render()),
                 hbox(text("Jumlah : "), jumlah->Render()), tambahBtn->Render(),
                 separator(),
                 hbox(text("Kata Kunci : "), input_pattern->Render()),
                 hbox(perbaruiBtn->Render(), hapusBtn->Render()), separator(),
                 sortirBtn->Render()}) |
           border;
  });
  auto view = Renderer(actions, [&] {
    auto table = Table(createTableData(head, pattern));
    table.SelectAll().Border();
    table.SelectRow(0).Decorate(bold);
    table.SelectAll().SeparatorVertical(LIGHT);
    table.SelectAll().SeparatorHorizontal(LIGHT);
    return table.Render();
  });
  auto root = Container::Horizontal({actions, Scroller(view)}) | borderEmpty;
  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(root);
}