;;; gtk-css.el --- Apply Gtk's application-level CSS styles to Emacs.

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

(defun gtk-css--load-module ()
  (unless (featurep 'gtk-css-sys)
    (if (and (featurep 'gtk)
             (string-prefix-p "3." gtk-version-string))
        (require 'gtk-css-sys)
      (error "Cannot load gtk-css: gtk3 version of Emacs required"))))

;;;###autoload
(defun gtk-css-compile-module ()
  nil)

;;;###autoload
(defun gtk-css-load-from-string (string)
  "Load Gtk CSS from STRING."
  (gtk-css--load-module)
  (gtk-css-sys-load-from-string string))


;;;###autoload
(defun gtk-css-load-from-file (path)
  "Load Gtk CSS from PATH."
  (with-temp-buffer
    (insert-file-contents-literally path)
    (gtk-css-load-from-string (buffer-string))))


(provide 'gtk-css)

