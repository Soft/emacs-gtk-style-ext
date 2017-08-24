;;; gtk-style-ext.el --- Extra control over gtk front-end's styles.

;; Copyright (C) 2017 Samuel Laurén <samuel.lauren@iki.fi>

;; Author: Samuel Laurén <samuel.lauren@iki.fi>
;; Version: 0.1

;; This file is NOT part of GNU Emacs.

;; This file is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; This file is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs; see the file COPYING. If not, write to
;; the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

(require 'cl-lib)
(require 'gtk-style-ext-sys)


;;;###autoload
(defun gtk-style-ext-load-from-string (string)
  "Load gtk CSS from STRING."
  (let ((provider (gtk-style-ext-sys-create-provider)))
    (gtk-style-ext-sys-provider-load-from-string provider string)
    (gtk-style-ext-sys-provider-activate
     provider
     gtk-style-ext-sys-provider-priority-application)))


;;;###autoload
(defun gtk-style-ext-load-from-file (path)
  "Load gtk CSS from PATH."
  (with-temp-buffer
    (insert-file-contents-literally path)
    (gtk-style-ext-load-from-string (buffer-string))))


;;;###autoload
(define-minor-mode gtk-style-ext-dark-theme-mode
  "Toggle preference for Gtk's dark theme variant.
With a prefix argument ARG, enable Dark Theme mode if ARG is
positive, and disable it otherwise. If called from Lisp, enable
the mode if ARG is omitted or nil.

This command applies to all frames that exist and frames to be
created in the future."
  :variable ((gtk-style-ext-sys-prefer-dark-theme-p)
             . (lambda (v) (gtk-style-ext-sys-prefer-dark-theme v))))


(defun gtk-style-ext--update-theme (&rest args)
  (let ((sheet (concat (gtk-style-ext--face-to-css "menubar" 'mode-line)
                       (gtk-style-ext--face-to-css "menu" 'menu))))
    (gtk-style-ext-load-from-string sheet)))


(defun gtk-style-ext--face-to-css (selector face)
  (gtk-style-ext--property-list-to-string
   selector
   (gtk-style-ext--face-to-property-list face)))


(defun gtk-style-ext--property-list-to-string (selector properties)
  (format "%s { %s }"
          selector
          (mapconcat (lambda (p) (format "%s: %s" (car p) (cdr p)))
                     properties
                     "; ")))


(defun gtk-style-ext--face-to-property-list (face)
  (cl-loop
   for (attr . nil) in (face-all-attributes face) ; Somehow this seem to list all the values as unspecified.
   for value = (face-attribute face attr)
   unless (eq value 'unspecified)
   append
   (pcase attr
     (:foreground `(("color" . ,value)))
     (:background `(("background-color" . ,value))))))


;; TODO: Maybe reset styles after disabling.
;;;###autoload
(define-minor-mode gtk-style-ext-adapt-to-theme-mode
  "Adapt Gtk's colors to current Emacs theme.
With a prefix argument ARG, enable Dark Theme mode if ARG is
positive, and disable it otherwise. If called from Lisp, enable
the mode if ARG is omitted or nil.

This command applies to all frames that exist and frames to be
created in the future."
  :lighter " adapt"
  :global t
  (if gtk-style-ext-adapt-to-theme-mode
      (progn
        (gtk-style-ext--update-theme)
        (advice-add 'load-theme :after #'gtk-style-ext--update-theme))
    (advice-remove 'load-theme #'gtk-style-ext--update-theme)))


(provide 'gtk-style-ext)

