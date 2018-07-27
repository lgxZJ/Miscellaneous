
;; Added by Package.el.  This must come before configurations of
;; installed packages.  Don't delete this line.  If you don't want it,
;; just comment it out by adding a semicolon to the start of the line.
;; You may delete these explanatory comments.
(package-initialize)

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(package-selected-packages
   (quote
    (ac-etags dumb-jump auto-complete go-complete flymake-go company-go ztree))))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )


;; load emacs 24's package system. Add MELPA repository.
(when (>= emacs-major-version 24)
  (require 'package)
  (add-to-list
   'package-archives
   ;; '("melpa" . "http://stable.melpa.org/packages/") ; many packages won't show if using stable
   '("melpa" . "http://melpa.milkbox.net/packages/")
   t))

;; WindMove
(when (fboundp 'windmove-default-keybindings)
  (windmove-default-keybindings))

;; WindMove key bindings
(global-set-key (kbd "C-c <left>")  'windmove-left)
(global-set-key (kbd "C-c <right>") 'windmove-right)
(global-set-key (kbd "C-c <up>")    'windmove-up)
(global-set-key (kbd "C-c <down>")  'windmove-down)

;; go-complete
(require 'go-complete)
(add-hook 'completion-at-point-functions 'go-complete-at-point)

;; auto-complete
(ac-config-default)

;; dumb-jump
(dumb-jump-mode)

;; ac-etags
(custom-set-variables
 '(ac-eags-requires 1))

(eval-after-load "etags"
  '(progn
     (ac-etags-setup)))

(defun my/c-mode-common-hook ()
  (add-to-list 'ac-sources 'ac-source-etags))

(add-hook 'c-mode-common-hook 'my/c-mode-common-hook)

;; key bindings
(global-set-key (kbd "M-,") 'godef-jump)
(global-set-key (kbd "M-.") 'pop-tag-mark)

;; ztree-dir
(setq ztree-dir-move-focus  't)

;; Which-Func-Mmode
(which-func-mode 1)
