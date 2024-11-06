프로젝트명 : 비트세이버 (메타 퀘스트2) <br>
개발기간 : 2023.09.05 ~ 2023.10.04<br>
개발인원 : 언리얼 개발자 2명<br>
시연 링크 :https://www.youtube.com/embed/onU5jmK92Ow <br>
<br>

# Point 1. DataTable을 이용한 음악 리스트 만들기
[SongListUI.cpp](https://github.com/micalia/BeatSaber_ShinSeolBin/blob/main/Source/BeatSaber/Private/SongListUI.cpp#L35)
<br>
#### View
<img src="https://github.com/user-attachments/assets/c1856309-5dc8-4577-89ff-dbddb79f2803" width="450" />

<br>
음악 리스트에 들어갈 음악 파일 정보가 담긴 csv파일을 언리얼에 임포트하여 DataTable로 변환한 후 해당 파일을 위젯이 생성될 때 데이터들을 파싱하여 음악 리스트 UI가 구성되도록 구현.

<br>

<발표자료>
<br>

![20231004193314_팀 그라데이션 베타_page-0001](https://github.com/user-attachments/assets/b7c91789-7074-4770-ab4f-3fb7d3cbaa3c)
![20231004193314_팀 그라데이션 베타_page-0002](https://github.com/user-attachments/assets/4d5c5e34-4f64-47d0-baf6-e6a3fe210cdf)
![20231004193314_팀 그라데이션 베타_page-0003](https://github.com/user-attachments/assets/ddbf8585-d3e3-4f84-a6c4-218e21345a7c)
![20231004193314_팀 그라데이션 베타_page-0004](https://github.com/user-attachments/assets/e0675286-0b38-4c07-8236-8c9f6516bfe9)
![20231004193314_팀 그라데이션 베타_page-0005](https://github.com/user-attachments/assets/a504ed80-aeb4-4a0b-b0d5-4738efb72d3a)
![20231004193314_팀 그라데이션 베타_page-0006](https://github.com/user-attachments/assets/f16a8625-44ba-49a6-86d9-29faca7c65e6)
![20231004193314_팀 그라데이션 베타_page-0007](https://github.com/user-attachments/assets/4bd1a91a-46b9-4997-b60f-d360e1879e08)
![20231004193314_팀 그라데이션 베타_page-0008](https://github.com/user-attachments/assets/1aa91595-c9fd-4690-88dd-183593f80731)
![20231004193314_팀 그라데이션 베타_page-0009](https://github.com/user-attachments/assets/58dec667-7faf-4fa8-a16c-fb04e52db223)
![20231004193314_팀 그라데이션 베타_page-0010](https://github.com/user-attachments/assets/268a8e27-c9b8-4a5a-83ff-809315179c6b)
![20231004193314_팀 그라데이션 베타_page-0011](https://github.com/user-attachments/assets/42583a7c-6ddc-4b65-a54e-936ecd4dd424)
![20231004193314_팀 그라데이션 베타_page-0012](https://github.com/user-attachments/assets/7e67a309-126a-4561-93e7-c34d14b64996)
![20231004193314_팀 그라데이션 베타_page-0013](https://github.com/user-attachments/assets/e093b7bb-9364-4111-8fd9-d0717d63992b)
![20231004193314_팀 그라데이션 베타_page-0014](https://github.com/user-attachments/assets/81c55d14-1a78-4224-aa02-baf17d3a400d)
![20231004193314_팀 그라데이션 베타_page-0015](https://github.com/user-attachments/assets/23aaaf81-b40b-466e-9935-3092b5792c43)
![20231004193314_팀 그라데이션 베타_page-0016](https://github.com/user-attachments/assets/00b43651-2385-4716-8ebf-61dde1f964e5)
![20231004193314_팀 그라데이션 베타_page-0017](https://github.com/user-attachments/assets/93fca671-8b7c-4821-a9b1-4559954e2c74)
![20231004193314_팀 그라데이션 베타_page-0018](https://github.com/user-attachments/assets/017c3545-8fae-4c8b-ae30-a94e386f4766)
![20231004193314_팀 그라데이션 베타_page-0019](https://github.com/user-attachments/assets/c625909c-18b5-42e0-9a62-5cd0f7de7489)
![20231004193314_팀 그라데이션 베타_page-0020](https://github.com/user-attachments/assets/e2d70097-c575-4f25-806b-6d4bb08f1c70)
![20231004193314_팀 그라데이션 베타_page-0021](https://github.com/user-attachments/assets/a225c766-551a-4f7b-967a-f2386e5072cf)
![20231004193314_팀 그라데이션 베타_page-0022](https://github.com/user-attachments/assets/b39842a5-0db0-4ccb-8ecd-5d8322fbd5c2)
![20231004193314_팀 그라데이션 베타_page-0023](https://github.com/user-attachments/assets/8b8f289e-75ac-4c99-8e08-29d55b246ec1)
![20231004193314_팀 그라데이션 베타_page-0024](https://github.com/user-attachments/assets/1156aac7-3512-4008-b628-bb30e4eefb68)
![20231004193314_팀 그라데이션 베타_page-0025](https://github.com/user-attachments/assets/d3d54288-fb0e-4ab7-a3c2-e9e60aa9ba2e)
