# Checking GOPATH
if [[ -z "${GOPATH}" ]]; then
    echo '!!NO GOPATH ENV SET!!'
    exit -1
fi

# Remove possibly already installed emacs versions
# Remove possibly installed emacs
rm -rf /usr/local/emacs
rm -rf ~/.emacs.d

# Make dir for emacs auto-build process
rm -rf lgx
mkdir -p lgx
cd lgx

# Uncompress tars
wget http://mirrors.ustc.edu.cn/gnu/emacs/emacs-26.1.tar.gz -O emacs.tar.gz
mkdir emacs
tar -zxvf emacs.tar.gz -C emacs --strip-components 1
cd emacs

# Install requirements
yum install gnutls gnutls-devel ncurses ncurses-devel -y

# Build emacs
./configure
make
make install bindir=/usr/local/emacs
make clean
make distclean
cd ..

# Add emacs bins to path, assumes ~/.bashrc exists
if [ ! -f ~/.bashrc ]; then
	touch ~/.bashrc
fi

cat >> ~/.bashrc << EOF
PATH=$PATH:/usr/local/emacs
EOF

# Change emacs to ems to avoid conflicts of other possibly installed emacs
mv /usr/local/emacs/emacs /usr/local/emacs/ems

# Go get [gocode, godef]
go get -u github.com/rogpeppe/godef
go get -u github.com/nsf/gocode

# Compile [guru, goimports] from source
git clone https://github.com/golang/tools.git
\cp -rf tools/* $GOPATH/src/golang.org/x/tools/

cd $GOPATH/src/golang.org/x/tools/cmd/guru/
go build .
cp ./guru $GOPATH/bin

cd ../goimports
go build .
cp ./goimports $GOPATH/bin

# Copy auto-install scripts into .emacs file under HOME dir
rm -f ~/.emacs
touch ~/.emacs

cat >> ~/.emacs << EOF
(require 'package)
(let* ((no-ssl (and (memq system-type '(windows-nt ms-dos))
                    (not (gnutls-available-p))))
       (proto (if no-ssl "http" "https")))
  (when no-ssl
    (warn "\
Your version of Emacs does not support SSL connections,
which is unsafe because it allows man-in-the-middle attacks.
There are two things you can do about this warning:
1. Install an Emacs version that does support SSL and be safe.
2. Remove this warning from your init file so you won't see it again."))
  ;; Comment/uncomment these two lines to enable/disable MELPA and MELPA Stable as desired
  (add-to-list 'package-archives (cons "melpa" (concat proto "://melpa.org/packages/")) t)
  ;;(add-to-list 'package-archives (cons "melpa-stable" (concat proto "://stable.melpa.org/packages/")) t)
  (when (< emacs-major-version 24)
    ;; For important compatibility libraries like cl-lib
    (add-to-list 'package-archives (cons "gnu" (concat proto "://elpa.gnu.org/packages/")))))
(package-initialize)
      

;; disable automatic loading of packages after the init file
   (setq package-enable-at-startup nil)
;; instead load them explicitly
   (package-initialize)
;; refresh package descriptions
   (unless package-archive-contents
    (package-refresh-contents))

;;; use-package initialization
;;; install use-package if not already done
    (if (not (package-installed-p 'use-package))
    (progn
      (package-refresh-contents)
	      (package-install 'use-package)))
	;;; use-package for all others
    (require 'use-package)

;; install your packages
(use-package go-mode
  :ensure t)
(use-package company-go
  :ensure t)
(use-package flycheck
  :ensure t)
(use-package ztree
  :ensure t)
(use-package go-guru
  :ensure t)
(use-package atom-dark-theme
  :ensure t)

;; enable company-mode by default
(add-hook 'after-init-hook 'global-company-mode)
;; enable flycheck-mode by default
(global-flycheck-mode)
;; enable electric-pair-mode by default
(electric-pair-mode 1)
;; enable ztree focus settings
(setq ztree-dir-move-focus t)
;; enable go-guru by default
(require 'go-guru)
(add-hook 'go-mode-hook #'go-guru-hl-identifier-mode)

;; Call Gofmt before saving
(setq gofmt-command "goimports")
(add-hook 'before-save-hook 'gofmt-before-save)

;; Key bindings specific to go-mode
(global-set-key (kbd "M-,") 'godef-jump)         ; Go to definition
(global-set-key (kbd "M-.") 'pop-tag-mark)       ; Return from whence you came
(global-set-key (kbd "M-/") 'compile)            ; Invoke compiler
(global-set-key (kbd "M-i") 'ztree-dir)          ; Invoke ztree-dir

(load-theme 'atom-dark t)

EOF
