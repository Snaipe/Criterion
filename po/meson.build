gettext = find_program('gettext', required: get_option('i18n'))

if gettext.found()
	i18n = import('i18n')
	config.set('ENABLE_NLS', true)

	add_project_arguments('-DGETTEXT_PACKAGE="criterion"', language: 'c')
	i18n.gettext(meson.project_name(),
		args: '--directory=' + meson.source_root()
	)
endif
