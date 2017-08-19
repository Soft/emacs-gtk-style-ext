
(defun gtk-css--load-module ()
  (unless (featurep 'gtk-css-sys)
    (if (and (featurep 'gtk)
             (string-prefix-p "3." gtk-version-string))
        (require 'gtk-css-sys)
      (error "Cannot load gtk-css: gtk3 version of Emacs required"))))


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

