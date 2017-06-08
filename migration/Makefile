all: migration_mbind migration_numa_move_pages \
	thp_migration_mbind thp_migration_numa_move_pages \
	migration_mbind_mt migration_numa_move_pages_mt \
	thp_migration_mbind_mt thp_migration_numa_move_pages_mt \
	thp_migration_mbind_mt_concur thp_migration_numa_move_pages_mt_concur \
	thp_migration_mbind_mt_concur_file thp_migration_mbind_mt_concur_htlb \
	thp_migration_mbind_mt_concur_mixed hugetlb_migration_mbind \
	hugetlb_migration_mbind_file hugetlb_migration_soffline \
	hugetlb_migration_soffline_file migration_soffline migration_soffline_file \
	migration_hoffline migration_hoffline_file

migration_mbind: migration_mbind.c
	cc -o migration_mbind migration_mbind.c -lnuma

migration_numa_move_pages: migration_numa_move_pages.c
	cc -o migration_numa_move_pages migration_numa_move_pages.c -lnuma

thp_migration_mbind: thp_migration_mbind.c
	cc -o thp_migration_mbind thp_migration_mbind.c -lnuma

thp_migration_numa_move_pages: thp_migration_numa_move_pages.c
	cc -o thp_migration_numa_move_pages thp_migration_numa_move_pages.c -lnuma

migration_mbind_mt: migration_mbind_mt.c
	cc -o migration_mbind_mt migration_mbind_mt.c -lnuma

migration_numa_move_pages_mt: migration_numa_move_pages_mt.c
	cc -o migration_numa_move_pages_mt migration_numa_move_pages_mt.c -lnuma

thp_migration_mbind_mt: thp_migration_mbind_mt.c
	cc -o thp_migration_mbind_mt thp_migration_mbind_mt.c -lnuma

thp_migration_numa_move_pages_mt: thp_migration_numa_move_pages_mt.c
	cc -o thp_migration_numa_move_pages_mt thp_migration_numa_move_pages_mt.c -lnuma

thp_migration_mbind_mt_concur: thp_migration_mbind_mt_concur.c
	cc -o thp_migration_mbind_mt_concur thp_migration_mbind_mt_concur.c -lnuma

thp_migration_mbind_mt_concur_file: thp_migration_mbind_mt_concur_file.c
	cc -o thp_migration_mbind_mt_concur_file thp_migration_mbind_mt_concur_file.c -lnuma

thp_migration_mbind_mt_concur_htlb: thp_migration_mbind_mt_concur_htlb.c
	cc -o thp_migration_mbind_mt_concur_htlb thp_migration_mbind_mt_concur_htlb.c -lnuma

thp_migration_mbind_mt_concur_mixed: thp_migration_mbind_mt_concur_mixed.c
	cc -o thp_migration_mbind_mt_concur_mixed thp_migration_mbind_mt_concur_mixed.c -lnuma

thp_migration_numa_move_pages_mt_concur: thp_migration_numa_move_pages_mt_concur.c
	cc -o thp_migration_numa_move_pages_mt_concur thp_migration_numa_move_pages_mt_concur.c -lnuma

hugetlb_migration_mbind: hugetlb_migration_mbind.c
	cc -o hugetlb_migration_mbind hugetlb_migration_mbind.c -lnuma

hugetlb_migration_mbind_file: hugetlb_migration_mbind_file.c
	cc -o hugetlb_migration_mbind_file hugetlb_migration_mbind_file.c -lnuma

hugetlb_migration_soffline: hugetlb_migration_soffline.c
	cc -o hugetlb_migration_soffline hugetlb_migration_soffline.c -lnuma

hugetlb_migration_soffline_file: hugetlb_migration_soffline_file.c
	cc -o hugetlb_migration_soffline_file hugetlb_migration_soffline_file.c -lnuma

migration_soffline: migration_soffline.c
	cc -o migration_soffline migration_soffline.c -lnuma

migration_soffline_file: migration_soffline_file.c
	cc -o migration_soffline_file migration_soffline_file.c -lnuma

migration_hoffline: migration_hoffline.c
	cc -o migration_hoffline migration_hoffline.c -lnuma

migration_hoffline_file: migration_hoffline_file.c
	cc -o migration_hoffline_file migration_hoffline_file.c -lnuma

test:
	./move
	./move_mt
clean:
	rm migration_mbind migration_numa_move_pages \
	thp_migration_mbind thp_migration_numa_move_pages \
	migration_mbind_mt migration_numa_move_pages_mt \
	thp_migration_mbind_mt thp_migration_numa_move_pages_mt \
	thp_migration_mbind_mt_concur thp_migration_numa_move_pages_mt_concur \
	thp_migration_mbind_mt_concur_file thp_migration_mbind_mt_concur_htlb \
	thp_migration_mbind_mt_concur_mixed hugetlb_migration_mbind \
	hugetlb_migration_mbind_file hugetlb_migration_soffline \
	hugetlb_migration_soffline_file migration_soffline migration_soffline_file \
	migration_hoffline migration_hoffline_file
