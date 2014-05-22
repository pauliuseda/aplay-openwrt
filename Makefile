#
# Copyright (C) 2011 OpenWrt.org  
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
# updated to work with latest source from abrasive
#

include $(TOPDIR)/rules.mk

PKG_NAME:=aplay
PKG_VERSION:=0
PKG_RELEASE:=$(PKG_SOURCE_VERSION)

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/pauliuseda/aplay.git
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz


PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk


define Package/aplay_new/Default
  SECTION:=sound
  CATEGORY:=Sound
  TITLE:=aplay_new
  URL:=http://mafipulation.org/
endef


define Package/aplay_new
  $(Package/aplay/Default)
#  DEPENDS:= +libpthread +libopenssl +alsa-lib +libavahi-client +avahi-utils +libao
   DEPENDS:= +lasound +lm 
endef

define Package/aplay_new/description
  ShairPort is server software that implements the RAOP protocol for
  playback of music streamed from a compatible remote client.
endef


define Package/aplay_new/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/aplay $(1)/usr/bin/
	
endef


$(eval $(call BuildPackage,aplay_new))
