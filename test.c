#include <stdio.h>
#include <windows.h>

// Test việc chạy tiến trình

int main() {
  DWORD pid = GetCurrentProcessId();
  printf("\n[Test Process %lu] Bat dau ngu trong 100 giay...\n", pid);

  // Ngủ 10 giây để bạn có đủ thời gian gõ lệnh 'list'
  Sleep(100000);

  printf("\n[Test Process %lu] Day roi! Dang thuc day va ket thuc.\n", pid);
  return 0;
}
