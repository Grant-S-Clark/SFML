git:
	find . -name '*.out' -exec rm -f {} \;
	find . -name '*.exe' -exec rm -f {} \;
	find . -name '*app' -exec rm -f {} \;
	find . -name '#*#'   -exec rm -f {} \;
	find . -type d -empty -delete
	git add *; git commit -m 'no msg'; git push
