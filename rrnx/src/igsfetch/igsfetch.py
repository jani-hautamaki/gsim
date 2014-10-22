#*******************************{begin:header}*******************************#
#                       rrnx - The Robust RINEX Library
#****************************************************************************#
#
#      Part of the GPS/INS measurement simulation system GSIM
#      https://code.google.com/p/gsim
#
#      Copyright (C) 2013-2014 Jani Hautamaki <jani.hautamaki@hotmail.com>
#
#      Licensed under the terms of GNU General Public License v3.
#
#       You should have received a copy of the GNU General Public License v3
#       along with this program as the file LICENSE.txt; if not, please see
#       http://www.gnu.org/licenses/gpl-3.0.html
#
#*********************************{end:header}*******************************#

# Example usage.
#
# To read all broadcast ephemerides for one month, 
# you can use the following command:
#
#     python igsfetch.py 2014-05-01 2014-06-01 ign > script.sh
#
# or
#
#     python igsfetch.py 2014-05-01 2014-06-01 ign - > urls.txt
#
# or
#
#     python igsfetch.py 2013-01-01 2014-01-01 ign - | awk '!(NR % 7)
#
# The awk in the last command will cause the every 7th line to be selected.
# This is an easy way to pick only one day of each week.
#

# built-in

import sys
from datetime import datetime

# 3rd party

# easy_install jdcal
import jdcal


def date2mjday(d):
	return int(jdcal.gcal2jd(d.year, d.month, d.day)[1])

def build_urls_for(year, yday):
	urls = {}

	yyyy = "%04d" % (year, )
	yy = "%02d" % (year % 100, )
	ddd = "%03d" % (yday, )

	keys = {
	    'yyyy' : yyyy,
	    'yy' : yy,
	    'ddd' : ddd,
	}

	# IGN (Paris, FR)
	ign_fname = "{site}{ddd}0.{yy}n.Z".format(site='brdc', **keys)
	ign_fpath = "{yyyy}/{ddd}/".format(**keys)
	ign_baseurl = "ftp://igs.ensg.ign.fr/pub/igs/data/" 
	urls['ign'] = ign_baseurl + ign_fpath + ign_fname

	# CDDIS (Greenbelt, MD)
	cddis_fname = "{site}{ddd}0.{yy}n.Z".format(site='brdc', **keys)
	cddis_fpath = "{yyyy}/{ddd}/{yy}n/".format(**keys)
	cddis_baseurl = "ftp://cddis.gsfc.nasa.gov/gps/data/daily/"
	urls['cddis'] = cddis_baseurl + cddis_fpath + cddis_fname

	# SOPAC (San Diego, CA)
	sopac_fname = "{site}{ddd}0.{yy}n.Z".format(site='auto', **keys)
	sopac_fpath = "{yyyy}/{ddd}/".format(**keys)
	sopac_baseurl = "ftp://garner.ucsd.edu/pub/rinex/"
	urls['sopac'] = sopac_baseurl + sopac_fpath + sopac_fname

	# KASI (Daejon, Republic of Korea)
	kasi_fname = "{site}{ddd}0.{yy}n.Z".format(site='brdc', **keys)
	kasi_fpath = "{yyyy}/{ddd}/{yy}n/".format(**keys)
	kasi_baseurl = "ftp://nfs.kasi.re.kr/gps/data/daily/"
	urls['kasi'] = kasi_baseurl + kasi_fpath + kasi_fname

	# IGS (Pasadena, CA)
	# (No RINEX data; only SP3 products)

	return urls
# ends: build_urls_for()

def display_usage(program):
	print "Usage:"
	print "    %s <from_date> <to_date> <site_abbr> [command...]" % (
            program, )
	print
	print "The dates, <from_date> and <to_date> are expected"
	print "to be specified in format yyyy-mm-dd."
	print
	print "The site abbreviation, <site_abbr>, is expected to be"
	print "one of the pre-defined values: ign, cddis, sopac, kasi"
	print
	print "The command is the command prefix used for the URLs."
	print "If the command is '-' then only the URLs are listed."
	print "Default value: 'wget -nv'"

def main(argv):
	if len(argv) < 4:
		print "Error: not enough arguments"
		print
		display_usage(argv[0])
		return 1

	from_date = datetime.strptime(argv[1], u"%Y-%m-%d")
	to_date = datetime.strptime(argv[2], u"%Y-%m-%d")
	site_abbr = argv[3]
	cmd = 'wget -nv'
	if len(argv) >= 5:
		cmd = ' '.join(argv[4:])
		#print "# Command = <%s>" % (cmd, )

	list_only = None
	if cmd != '-':
		list_only = False
		cmd = cmd + ' '
	else:
		list_only = True
		cmd = ''
	# if-else

	# Convert to Julian Day numbers
	from_mjday = date2mjday(from_date)
	to_mjday = date2mjday(to_date)

	# Compute the GPS zero day
	gps_zeroday = date2mjday(datetime(1980, 1, 6))

	if list_only == False:
		print "#!/bin/bash"
		print
		print "# Automatically generated script for downloading "\
		    "RINEX NAV files."
		print "# From date:   %s" % (from_date.strftime("%Y-%m-%d"), )
		print "# To date:     %s (excluded)" % (to_date.strftime("%Y-%m-%d"), )
		print "# Span:        %d days" % (to_mjday - from_mjday, )
		print
	# if: not list_only

	# Loop from the first day until the last day (exclusive)
	for mjday in range(from_mjday, to_mjday):
		gpsday = mjday - gps_zeroday
		gdate = datetime(*jdcal.jd2gcal(jdcal.MJD_0, mjday)[:3])
		year = gdate.year

		# Compute the mjd for the first day of the year
		yday_zero = date2mjday(datetime(year, 1, 1))
		yday = mjday - yday_zero + 1

		# Get the GPS week number
		gpscycle = int(gpsday / (1024*7))
		gpsweek = int(gpsday / 7) % 1024
		gpsdow = gpsday % 7

		# Compute day of the year

		"""
		print "# %s is gpsday %s (cycle %d week %d dow %d; yday %d)" % (
		    gdate.strftime("%Y-%m-%d"), 
		    gpsday,
		    gpscycle,
		    gpsweek,
		    gpsdow,
		    yday,
		)
		"""

		# Select correct base url.
		urls = build_urls_for(year, yday)

		print "%s%s" % (cmd, urls[site_abbr],)	
	# for
# main()

if __name__ == '__main__':
	sys.exit(main(sys.argv))
