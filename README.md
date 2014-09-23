# Notepad--

  Simple notepad implementation that supports UTF-8 only.
At first Notepad--'s goal was to give a user the possibility
to edit the files of any size. User would be able to go to the middle
of the huge file and make changes. But after I've aleady written some
of the core classes I found that there is no such editing control that
would allow the desired behavior. Standart Edit and RichEdit do not
suit at all. All of them store the textual data by themselves and want
that data to be at hand in the RAM. No interface to delegate the task
of data managing to my code. Even advanced non-standart editing
components lack such feature.

  Attempts to modify the data held by any of the controls, (adding at the
end, inserting at the beginning) for simulating large piece of text, failed.
It breaks "undo history", resets user selection and so on. This approach
requires too many hacks and will barely provide good user impressions.

  Creating own editing control isn't so easy. If doing so, I would have
to fully reimplement existing standart Edit control plus very deliberate
callback API would has to be designed to let the control's container
manage the data. Such things have to pass through different stages of
development, including the creation of a prototype and analyzing it.

  Current Notepad-- uses the same Scintilla control as his mature 
brother (Notepad++) does. Scintilla was chosen because it allows to
append the text to the active editing buffer without breaking anything.
So a user can start editing while the text is still loading.

  But **really big files** are still the problem.

Solution consists of three projects:
+ Core library
+ Scintilla component
+ User interface
